#include <stdio.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

void parseTable(xmlNode *node) {
    xmlNode *cur_node = NULL;
    for (cur_node = node; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE && xmlStrcmp(cur_node->name, (const xmlChar *)"tr") == 0) {
            xmlNode *td_node = NULL;
            for (td_node = cur_node->children; td_node; td_node = td_node->next) {
                if (td_node->type == XML_ELEMENT_NODE && xmlStrcmp(td_node->name, (const xmlChar *)"td") == 0) {
                    xmlNode *text_node = td_node->children;
                    if (text_node && text_node->type == XML_TEXT_NODE) {
                        printf("%s: %s\n", td_node->name, text_node->content);
                    }
                }
            }
        }
        parseTable(cur_node->children);
    }
}

int main()
{
    xmlDoc *doc = NULL;
    xmlNode *root = NULL;

    doc = xmlReadFile("table.xml", NULL, 0);
    if (doc == NULL) {
        fprintf(stderr, "Failed to parse document");
        return 1;
    }

    root = xmlDocGetRootElement(doc);
    if (root == NULL) {
        fprintf(stderr, "Empty document");
        xmlFreeDoc(doc);
        return 1;
    }

    parseTable(root);

    xmlFreeDoc(doc);
    xmlCleanupParser();

    return 0;
}
