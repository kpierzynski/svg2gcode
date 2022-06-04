#include "loader.h"

xmlDocPtr getdoc(char *docname)
{
	xmlDocPtr doc;
	doc = xmlParseFile(docname);

	if (doc == NULL)
	{
		fprintf(stderr, "Document not parsed successfully. \n");
		return NULL;
	}
	return doc;
}

xmlXPathObjectPtr getnodeset(xmlDocPtr doc, xmlChar *xpath)
{

	xmlXPathContextPtr context;
	xmlXPathObjectPtr result;

	context = xmlXPathNewContext(doc);
	if (context == NULL)
	{
		fprintf(stderr, "Error in xmlXPathNewContext\n");
		return NULL;
	}
	result = xmlXPathEvalExpression(xpath, context);
	xmlXPathFreeContext(context);
	if (result == NULL)
	{
		fprintf(stderr, "Error in xmlXPathEvalExpression\n");
		return NULL;
	}
	if (xmlXPathNodeSetIsEmpty(result->nodesetval))
	{
		xmlXPathFreeObject(result);
		fprintf(stderr, "No result\n");
		return NULL;
	}
	return result;
}

void load_paths(const char *path, void(callback)(char *s))
{
	xmlDocPtr doc;
	xmlChar *xpath;
	xmlNodeSetPtr nodeset;
	xmlXPathObjectPtr result;
	int i;

	doc = getdoc((char *)path);
	float x, y, view_width, view_height;
	float width, height;

	// Warning, viewBox can be separated by spaces, commas or both!
	sscanf((char *)xmlGetProp(doc->children, (xmlChar *)"viewBox"), "%f %f %f %f", &x, &y, &view_width, &view_height);
	sscanf((char *)xmlGetProp(doc->children, (xmlChar *)"width"), "%f", &width);
	sscanf((char *)xmlGetProp(doc->children, (xmlChar *)"height"), "%f", &height);

	set_function_y(height / view_height, -y);
	set_function_x(width / view_width, -x);

	// path d
	// polyline points

	xpath = (xmlChar *)"//*[local-name(.) = 'path']";
	result = getnodeset(doc, xpath);
	if (result)
	{
		nodeset = result->nodesetval;
		for (i = 0; i < nodeset->nodeNr; i++)
		{
			if (callback)
				callback((char *)xmlGetProp(nodeset->nodeTab[i], (xmlChar *)"d"));
		}
		xmlXPathFreeObject(result);
	}

	xpath = (xmlChar *)"//*[local-name(.) = 'polyline']";
	result = getnodeset(doc, xpath);
	if (result)
	{
		nodeset = result->nodesetval;
		for (i = 0; i < nodeset->nodeNr; i++)
		{
			if (callback)
			{
				char *points = (char *)xmlGetProp(nodeset->nodeTab[i], (xmlChar *)"points");
				int len = strlen(points);
				char *data = (char *)malloc(sizeof(char) * (len + 2));
				data[0] = 'M';
				data[len+1] = '\0';
				memcpy(data + 1, points, len);
				callback(data);
				free(data);
			}
		}
		xmlXPathFreeObject(result);
	}

	xpath = (xmlChar *)"//*[local-name(.) = 'polygon']";
	result = getnodeset(doc, xpath);
	if (result)
	{
		nodeset = result->nodesetval;
		for (i = 0; i < nodeset->nodeNr; i++)
		{
			if (callback)
			{
				char *points = (char *)xmlGetProp(nodeset->nodeTab[i], (xmlChar *)"points");
				int len = strlen(points);
				char *data = (char *)malloc(sizeof(char) * (len + 3));
				data[0] = 'M';
				data[len+1] = 'Z';
				data[len+2] = '\0';
				memcpy(data + 1, points, len);
				callback(data);
				free(data);
			}
		}
		xmlXPathFreeObject(result);
	}

	xmlFreeDoc(doc);
	xmlCleanupParser();
}
