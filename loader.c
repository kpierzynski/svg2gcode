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
			
		}
		xmlXPathFreeObject(result);
	}
	xmlFreeDoc(doc);
	xmlCleanupParser();
}
