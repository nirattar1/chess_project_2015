/*
 * Test_Files.c
 *
 *  Created on: Oct 2, 2015
 *      Author: nirattar
 */

#include "../Files.h"

void Test_Files ()
{
	xml_create_tree();
}



#if defined(LIBXML_TREE_ENABLED) && defined(LIBXML_OUTPUT_ENABLED)

/*
 *To compile this file using gcc you can type
 *gcc `xml2-config --cflags --libs` -o tree2 tree2.c
 */

/* A simple example how to create DOM. Libxml2 automagically
 * allocates the necessary amount of memory to it.
*/
int xml_create_tree()
{
    xmlDocPtr doc = NULL;       /* document pointer */
    xmlNodePtr root_node = NULL, node = NULL, node1 = NULL;/* node pointers */
    char buff[256];
    int i, j;

    LIBXML_TEST_VERSION;

    /*
     * Creates a new document, a node and set it as a root node
     */
    doc = xmlNewDoc(BAD_CAST "1.0");
    root_node = xmlNewNode(NULL, BAD_CAST "game");
    xmlDocSetRootElement(doc, root_node);


    /*
     * xmlNewChild() creates a new node, which is "attached" as child node
     * of root_node node.
     */
    xmlNewChild(root_node, NULL, BAD_CAST "node1",
                BAD_CAST "content of node 1");
    /*
     * The same as above, but the new child node doesn't have a content
     */
    xmlNewChild(root_node, NULL, BAD_CAST "node2", NULL);

//    /*
//     * Here goes another way to create nodes. xmlNewNode() and xmlNewText
//     * creates a node and a text node separately. They are "attached"
//     * by xmlAddChild()
//     */
//    node = xmlNewNode(NULL, BAD_CAST "node4");
//    node1 = xmlNewText(BAD_CAST
//                   "other way to create content (which is also a node)");
//    xmlAddChild(node, node1);
//    xmlAddChild(root_node, node);


    /*
     * Dumping document to stdio or file
     */
    xmlSaveFormatFileEnc("test.xml", doc, "UTF-8", 1);

    /*free the document */
    xmlFreeDoc(doc);

    /*
     *Free the global variables that may
     *have been allocated by the parser.
     */
    xmlCleanupParser();

    /*
     * this is to debug memory for regression tests
     */
    xmlMemoryDump();
    return(0);
}
#else
int xml_create_tree() {
    fprintf(stderr, "tree support not compiled in\n");
    exit(1);
}
#endif
