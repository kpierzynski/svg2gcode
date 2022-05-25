#include "loader.h"

xmlDocPtr getdoc (char *docname) {
	xmlDocPtr doc;
	doc = xmlParseFile(docname);
	
	if (doc == NULL ) {
		fprintf(stderr,"Document not parsed successfully. \n");
		return NULL;
	}

  printf("%s\r\n", doc->children->name);
	return doc;
}

xmlXPathObjectPtr getnodeset (xmlDocPtr doc, xmlChar *xpath){
	
	xmlXPathContextPtr context;
	xmlXPathObjectPtr result;

	context = xmlXPathNewContext(doc);
	if (context == NULL) {
		printf("Error in xmlXPathNewContext\n");
		return NULL;
	}
	result = xmlXPathEvalExpression(xpath, context);
	xmlXPathFreeContext(context);
	if (result == NULL) {
		printf("Error in xmlXPathEvalExpression\n");
		return NULL;
	}
  printf("%d\r\n", result->nodesetval->nodeNr);
	if(xmlXPathNodeSetIsEmpty(result->nodesetval)){
		xmlXPathFreeObject(result);
    printf("No result\n");
		return NULL;
	}
	return result;
}

void load_svg(const char * path) {
	xmlDocPtr doc;
	xmlChar *xpath = (xmlChar*) "//*[local-name(.) = 'path']";
	xmlNodeSetPtr nodeset;
	xmlXPathObjectPtr result;
	int i;

	doc = getdoc((char*)path);
	result = getnodeset (doc, xpath);
	if (result) {
		nodeset = result->nodesetval;
		for (i=0; i < nodeset->nodeNr; i++) {
  		printf("element: %s, d attrib: %s\n", nodeset->nodeTab[i]->name, xmlGetProp(nodeset->nodeTab[i], (xmlChar*)"d") );

		}
		xmlXPathFreeObject (result);
	}
	xmlFreeDoc(doc);
	xmlCleanupParser();

}

