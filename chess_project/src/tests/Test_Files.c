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

	xml_read_from_tree();
}




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



/**
 * section: Tree
 * synopsis: Navigates a tree to print element names
 * purpose: Parse a file to a tree, use xmlDocGetRootElement() to
 *          get the root element, then walk the document and print
 *          all the element name in document order.
 * usage: tree1 filename_or_URL
 * test: tree1 test2.xml > tree1.tmp && diff tree1.tmp $(srcdir)/tree1.res
 * author: Dodji Seketeli
 * copy: see Copyright for the status of this software.
 */


/*
 *To compile this file using gcc you can type
 *gcc `xml2-config --cflags --libs` -o xmlexample libxml2-example.c
 */

/**
 * print_element_names:
 * @a_node: the initial xml node to consider.
 *
 * Prints the names of the all the xml elements
 * that are siblings or children of a given xml node.
 */
static void
print_element_names(xmlNode * a_node)
{
    xmlNode *cur_node = NULL;

    for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
            printf("node type: Element, name: %s\n", cur_node->name);
        }

        print_element_names(cur_node->children);
    }
}


/**
 * Simple example to parse a file called "file.xml",
 * walk down the DOM, and print the name of the
 * xml elements nodes.
 */
int
xml_read_from_tree(int argc, char **argv)
{
    xmlDoc *doc = NULL;
    xmlNode *root_element = NULL;


    /*
     * this initialize the library and check potential ABI mismatches
     * between the version it was compiled for and the actual shared
     * library used.
     */
    LIBXML_TEST_VERSION

    /*parse the file and get the DOM */
	char * filename = "test.xml";
    doc = xmlReadFile(filename, NULL, 0);

    if (doc == NULL) {
        printf("error: could not parse file %s\n", filename);
    }

    /*Get the root element node */
    root_element = xmlDocGetRootElement(doc);

    print_element_names(root_element);

    /*free the document */
    xmlFreeDoc(doc);

    /*
     *Free the global variables that may
     *have been allocated by the parser.
     */
    xmlCleanupParser();

    return 0;
}


