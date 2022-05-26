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
		printf("Error in xmlXPathNewContext\n");
		return NULL;
	}
	result = xmlXPathEvalExpression(xpath, context);
	xmlXPathFreeContext(context);
	if (result == NULL)
	{
		printf("Error in xmlXPathEvalExpression\n");
		return NULL;
	}
	if (xmlXPathNodeSetIsEmpty(result->nodesetval))
	{
		xmlXPathFreeObject(result);
		printf("No result\n");
		return NULL;
	}
	return result;
}

void load_paths(const char *path, void(callback)(char *s))
{
	xmlDocPtr doc;
	xmlChar *xpath = (xmlChar *)"//*[local-name(.) = 'path']";
	xmlNodeSetPtr nodeset;
	xmlXPathObjectPtr result;
	int i;

	doc = getdoc((char *)path);
	result = getnodeset(doc, xpath);
	if (result)
	{
		nodeset = result->nodesetval;
		for (i = 0; i < nodeset->nodeNr; i++)
		{
			if( callback ) callback((char *)xmlGetProp(nodeset->nodeTab[i], (xmlChar *)"d"));
			return;
		}
		xmlXPathFreeObject(result);
	}
	xmlFreeDoc(doc);
	xmlCleanupParser();
}
