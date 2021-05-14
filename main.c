/**
 * encoding: utf-8
 * @author: Liu Wangwei
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//读取的文件最大容量 1M
#define MAX_SIZE (1024*1024)

/**
 * 单词统计时会出现的五种状态
 */
typedef enum {
    INIT_FLAG, SPACE_FLAG, SYMBOL_FLAG, CHAR_FLAG, END_FLAG
} FLAG;

int count_char(const char *str);

int count_word(const char *str);

char *read_file(char *file);

int parse_args(int *mode, char **filePath, int argn, char **args);

int main(int argn, char **args) {
    //选择的模式，1:计算字符， 2:计算单词
    int mode = 0;
    char *file;

    if (parse_args(&mode, &file, argn, args) == -1) {
        printf("error arguments");
        return -1;
    }

    char *content = read_file(file);
    if (content == NULL) {
        return -1;
    }

    if (mode == 1) {
        printf("char number：%d", count_char(content));
    } else if (mode == 2) {
        printf("word number：%d", count_word(content));
    }
    return 0;
}

/**
 * 计算字符数
 * @param str 字符串
 * @return 总字符数
 */
int count_char(const char *str) {
    int num = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        num++;
    }
    return num;
}

/**
 * 计算单词数
 * @param str 字符串
 * @return 单词总数
 */
int count_word(const char *str) {
    int num = 0;
    FLAG flag = INIT_FLAG;
    for (int i = 0; str[i] != '\0'; i++) {
        /**
         * 读取到对应的字符时，进入对应的状态，根据当前的状态进行对应的操作
         * 除CHAR_FLAG外，读取到字符时，单词数+1，即每次从非字符状态进入字符状态时，单词数+1
         * INIT_FLAG: 初始状态
         * SPACE_FLAG: 空格状态，读取到空格或回车时进入此状态
         * SYMBOL_FLAG: 逗号状态，读取到逗号时进入（如果新增其它分隔符时，也进入此状态）
         * CHAR_FLAG: 字符状态，读取到字符时进入
         * END_FLAG: 结束状态，读取到结束时进入此状态，且直接返回结果
         */
        char c = str[i];
        if (c == '\0') {
            flag = END_FLAG;
        }
        //初始状态
        if (flag == INIT_FLAG) {
            if (c == ' ' || c == '\n') {
                flag = SPACE_FLAG;
            } else if (c == ',') {
                flag = SYMBOL_FLAG;
            } else {
                flag = CHAR_FLAG;
                num++;
            }
        }//空格状态
        else if (flag == SPACE_FLAG) {
            if (c == ',') {
                flag = SYMBOL_FLAG;
            } else if (c == ' ' || c == '\n') {
                //状态未改变
            } else {
                flag = CHAR_FLAG;
                num++;
            }
        }//逗号状态
        else if (flag == SYMBOL_FLAG) {
            if (c == ' ' || c == '\n') {
                flag = SPACE_FLAG;
            } else if (c == ',') {
                //状态未改变
            } else {
                flag = CHAR_FLAG;
                num++;
            }
        } //字符状态
        else if (flag == CHAR_FLAG) {
            if (c == ' ' || c == '\n') {
                flag = SPACE_FLAG;
            } else if (c == ',') {
                flag = SYMBOL_FLAG;
            } else {
                //状态未改变
            }
        }//结束状态
        else {
            break;
        }
    }
    return num;

}

/**
 * 解析输入参数
 */
int parse_args(int *mode, char **filePath, int argn, char **args) {
    if (argn != 3) {
        return -1;
    }
    if (args[1][1] == 'c') {
        *mode = 1;
    } else if (args[1][1] == 'w') {
        *mode = 2;
    } else {
        return -1;
    }
    *filePath = args[2];
    return 1;
}

/**
 * 读取文件
 */
char *read_file(char *file) {
    FILE *fp = fopen(file, "r");
    if (fp == NULL) {
        printf("the file is not exist\n");
        return NULL;
    }
    char *content = (char *) malloc(sizeof(char) * MAX_SIZE + 1);
    content[0] = '\0';
    while (!feof(fp)) {
        char str[1024] = {'\0'};
//        fgets(str, 1024, fp);
        fread(str, 1024, 1, fp);
        strcat(content, str);
    }
    //最后一位为结束符
    content[MAX_SIZE] = '\0';
    fclose(fp);
    return content;
}
