#include <stdio.h>
#include <stdlib.h>

#ifndef max
#define max(a,b) ((a) > (b) ? (a) : (b))
#endif

typedef struct TreeNode
{
    struct TreeNode* left;
    struct TreeNode* right;
    int    reached;  /* �Ƿ�ɴ� */
    int fstate;      /* ��� */
    int sym;
}TreeNode;

typedef struct TableItem
{
    int fstate;   /* ��һ��״̬ */
    int output;   /* �Ƿ������ */
    int sym;      /* ������ַ� */
}TableItem;

void usage()
{
    printf("Usage: ffcp [Option]... InputFile OutputFile\n");
}

/*
 * ��һ������
 * ����ֵ:
 *    0 �ɹ�       *destLength ��ȡ�ĳ���
 *    1 buff����   *destLength ��ȡ�ĳ���
 *    2 ���ļ�β
 */
int readLine(FILE* file, char* buff, int length, int *destLength)
{
    char ch = getc(file);
    int count = 0;

    while ((ch != '\n') && (ch != EOF)) {
        buff[count] = ch;
        count++;

        ch = getc(file);
    }

    if (count == 0 && ch == EOF) {
        return 2;
    }
    *destLength = count;
    return 0;
}

/*
 * ���� node �ڵ����һ��
 */
void processNode(TreeNode* root, TreeNode* node, int* maxFstate, int bitCount, TableItem *table)
{
    for (int i = 0; i < (1 << bitCount); i++) {
        TreeNode* curr = node->left ? node : root;
        for (int j = bitCount - 1; j >= 0; j--) {
            if ((1<<j) & i) {
                curr = curr->right;
            } else {
                curr = curr->left;
            }
            if (table) {
                if (curr->fstate == -1 && curr->reached) {
                    *maxFstate = *maxFstate + 1;
                    curr->fstate = *maxFstate;
                }
            }
            if (!curr->left) {
                if (table) {
                    table[i].output = 1;
                    table[i].sym = curr->sym;
                }
                curr = root;
            }
        }

        if (table) {
            table[i].fstate = curr->fstate;
        } else {
            curr->reached = 1;
        }
    }
}

void travers(TreeNode* root, TreeNode* node, int* maxFstate, int bitCount, TableItem* table) {
    processNode(root, node, maxFstate, bitCount, table ? table + node->fstate*(1<<bitCount) : NULL);
    if (node->left) {
        travers(root, node->left, maxFstate, bitCount, table);
    }
    if (node->right) {
        travers(root, node->right, maxFstate, bitCount, table);
    }
}

int main(int argc, char** argv)
{
    if (argc < 3) {
        usage();
        return 1;
    }

    FILE* file = fopen(argv[argc - 2], "r");
    if (!file) {
        printf("can not open file: %s\n", argv[argc - 2]);
        return 1;
    }

    char buffer[2049];
    int length = 0;
    int lineCount = 0;
    int nodeCount = 1;
    int maxDepth = 0;
    TreeNode* root = calloc(1, sizeof(TreeNode));
    if (!root) {
        printf("OOM");
        fclose(file);
        return 1;
    }
    while (readLine(file, buffer, 2048, &length) == 0) {
        TreeNode* curr = root;
        int i = 0;
        for (; i < length && curr; i++) {
            switch (buffer[i])
            {
            case '0':
                if (curr->left) {
                    curr = curr->left;
                } else {
                    curr->left = calloc(1, sizeof(TreeNode));
                    curr = curr->left;
                    nodeCount++;
                    if (curr) {
                        curr->fstate = -1;
                    }
                }
                break;
            case '1':
                if (curr->right) {
                    curr = curr->right;
                }
                else {
                    curr->right = calloc(1, sizeof(TreeNode));
                    curr = curr->right;
                    nodeCount++;
                    if (curr) {
                        curr->fstate = -1;
                    }
                }
                break;
            default:
                // ignor other
                break;
            }
        }
        if (curr) {
            curr->sym = lineCount++;
        }
        maxDepth = max(maxDepth, i);
    }
    fclose(file);

    int bitCount = 4;
    int maxFstate = 0;
    // ÿ�ζ�4λ
    travers(root, root, NULL, bitCount, NULL);
    TableItem* table = calloc(nodeCount*(1<<bitCount), sizeof(TableItem));
    if (!table) {
        printf("OOM\n");
        return 1;
    }
    travers(root, root, &maxFstate, bitCount, table);

    // output table
    file = fopen(argv[argc - 1], "w+");
    if (!file) {
        printf("cant open file�� %s\n", argv[argc - 1]);
        return 1;
    }

    fprintf(file, "const huffman_decode_node huffman_decode_table[][%d] = {\n", 1 <<bitCount);
    for (int i = 0; i <= maxFstate; i++) {
        fprintf(file, "    /* %d */\n    {\n", i);
        TableItem* item = table + i * (1 << bitCount);
        for (int j = 0; j < 1<<bitCount; j++, item++) {
            fprintf(file, "        {0x%02x, %d, %d},\n", item->fstate, item->output, item->sym);
        }
        fprintf(file, "    },\n");
    }
    fprintf(file, "};");
    fclose(file);
    return 0;
}
