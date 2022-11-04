#define FsDebug_lib
#ifdef FsDebug_lib
/* 
 * File:   main.c
 * Author: fslib
 *
 * Created on 2013年3月7日, 下午4:00
 */
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "PublicTools/Fs/Config.h"
////////////////////////////////////////////////////////////////////////////////
///////////////////////////定义私有结构开始//////////////////////////////////////
///////////////////////////定义私有结构结束//////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///////////////////////////定义私有函数开始//////////////////////////////////////
///////////////////////////定义私有函数结束//////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///////////////////////////定义公共函数开始//////////////////////////////////////

int main(int argc, char** argv) {
    // 创建配置
    FsConfig * const pConfig = fs_Config_new__IO();
    // 创建一个节点型节点
    void *const node = fs_Config_node_node_add(pConfig, pConfig, "node", "节点", "测试节点", 0, 0x7);
    {
        /* 在node下创建一个字符串节点,节点可以设置2个值,长度为0到10个字节 */
        void *const testString = fs_Config_node_string_add(pConfig, node, "testString", "测试字符串", "测试字符串", 0, 0x7, 0, 10, 2);
        /* 为testString添加一个default1的默认值 */
        fs_Config_node_string_add_value(pConfig, testString, FsConfig_nodeValue_default, "default1", "默认值1", "默认值1");
        /* 为testString添加一个optiona1的可选值 */
        fs_Config_node_string_add_value(pConfig, testString, FsConfig_nodeValue_optional, "optiona1", "可选值1", "可选值1");
        /* 在node下创建一个整型节点,节点可以设置2个值,取值范围为0到666666 */
        void *const testInt = fs_Config_node_integer_add(pConfig, node, "testInt", "测试整数", "测试整数", FsConfig_nodeShowType_default, 0, 0x7, 0, 666666, 2);
        /* 为testInt添加一个0的默认值 */
        fs_Config_node_integer_add_value(pConfig, testInt, FsConfig_nodeValue_default, 0, "0", "0");
        /* 为testInt添加一个100的可选值 */
        fs_Config_node_integer_add_value(pConfig, testInt, FsConfig_nodeValue_optional, 100, "100", "100");
        /* 在node下创建一个浮点节点,节点可以设置3个值,取值范围为0.0到1.0 */
        void *const testFloat = fs_Config_node_float_add(pConfig, node, "testFloat", "测试浮点", "测试浮点", 0, 0x7, 0.0, 1.0, 3);
        /* 为testFloat添加一个0.0的默认值 */
        fs_Config_node_float_add_value(pConfig, testFloat, FsConfig_nodeValue_default, 0.0, "0.0", "0.0");
        /* 为testFloat添加一个1.0的可选值 */
        fs_Config_node_float_add_value(pConfig, testFloat, FsConfig_nodeValue_optional, 1.0, "1.0", "1.0");
        /* 在node下创建一个二进制节点,节点可以设置2个值,长度为0到100个字节 */
        void *const testBinary = fs_Config_node_binary_add(pConfig, node, "testBinary", "测试二进制", "测试二进制", 0, 0x7, 1, 100, 2);
        /* 为testBinary创建一个条件组 */
        void *const condition_testBinary = fs_Config_condition_group_add(pConfig, testBinary);
        /* 向condition_testBinary添加一个条件,相对于testBinary节点向上一级的父节点中查找testInt节点,在testInt的值为0时此值有效 */
        fs_Config_condition_add_static(pConfig, condition_testBinary, 1, "testInt", FsConfig_Condition_equal, "0");
    }
    // 创建一个可以创建100个实例的模板
    void *const template = fs_Config_node_template_add(pConfig, pConfig, "testTemplate", "测试模板", NULL, NULL, "测试模板", NULL, NULL, NULL, 0, 0x7, 100);
    {
        /* 在template下创建一个字符串节点,节点可以设置2个值,长度为0到10个字节 */
        void *const testString1 = fs_Config_node_string_add(pConfig, template, "testString1", "测试字符串1", "测试字符串1", 0, 0x7, 0, 10, 2);
        /* 为testString1添加一个默认值 */
        fs_Config_node_string_add_value(pConfig, testString1, FsConfig_nodeValue_default, "default1", "默认值1", "默认值1");
        /* 为testString1添加一个可选值 */
        fs_Config_node_string_add_value(pConfig, testString1, FsConfig_nodeValue_optional, "optiona1", "可选值1", "可选值1");
        /* 在template下创建一个字符串节点,节点可以设置2个值,长度为0到10个字节 */
        void *const testString2 = fs_Config_node_string_add(pConfig, template, "testString2", "测试字符串2", "测试字符串2", 0, 0x7, 0, 10, 2);
        /* 为testString2创建一个条件组 */
        void *const condition_testString2 = fs_Config_condition_group_add(pConfig, testString2);
        /* 向condition_testString2添加一个条件,相对于testString2节点向上两级的父节点中查找node节点,再在node节点中查找testInt节电,在testInt的值为0时此值有效 */
        fs_Config_condition_add_static(pConfig, condition_testString2, 2, "node testInt", FsConfig_Condition_equal, "0");
    }
    // 把配置保存到文件,可使用小雉配置工具打开编辑
    // 项目中可把配置发送给客户端
    fs_Config_save_to_file_direct(pConfig, "test.cfg");
    /* 定义一个xml模拟历史数据导入 */
    {
        const char *str = "<testTemplate><testString1>sss1</testString1></testTemplate>"
                "<testTemplate><testString2>ssss2</testString2></testTemplate>"
                "<node><testInt>30</testInt><testFloat>0.5</testFloat></node>";
        FsXml *pXml = fs_Xml_new_from_string__IO(str, NULL);
        fs_Xml_analyzeAll(pXml, (struct FsXml_node*) pXml, NULL);
        FsEbml *pEbml1 = fs_Ebml_new_from_Xml__IO(pXml);
        fs_Xml_delete__OI(pXml, NULL);
        fs_Config_import_onlyData((FsEbml*) pConfig, (struct FsEbml_node*) pConfig, (struct FsEbml_node*) pConfig, (FsEbml*) pEbml1, (struct FsEbml_node*) pEbml1, NULL);
        fs_Ebml_delete__OI(pEbml1, NULL);
    }
    // 当前pConfig已包含导入的数据,可存盘
    fs_Config_save_to_file_direct(pConfig, "test1.cfg");
    // 读取历史的配置文件"test1.cfg",按目前在申明导入到pConfig中
    {
        FsConfig * const pConfig1 = fs_Config_new_from_file__IO("test1.cfg", NULL);
        fs_Config_import_onlyData((FsEbml*) pConfig, (struct FsEbml_node*) pConfig, (struct FsEbml_node*) pConfig, (FsEbml*) pConfig1, (struct FsEbml_node*) pConfig1, NULL);
        fs_Config_delete__OI(pConfig1, NULL);
    }
    /* 把pConfig导出为json */
    FsObjectBase * const pObjectBase = fs_Config_export_objectBase__IO(pConfig, FsConfig_ExportType_json_export, sizeof (FsObjectBase), 0, NULL);
    printf("%s\n", pObjectBase->data);
    /* 打印数据为:
     * {
     *     "node":{
     *         "testString":"default1",
     *         "testInt":"30",
     *         "testFloat":"0.500000",
     *         "testBinary":""
     *     },
     *     "testTemplate":[{
     *         "testString1":"sss1",
     *         "testString2":""
     *     },{
     *         "testString1":"default1",
     *         "testString2":"ssss2"
     *     }]
     * }
     */
    /* 读取node testInt的值,打印结果为testInt=30 */
    printf("testInt=%lld\n", fs_Config_node_integer_get_first(pConfig, pConfig, pConfig, "node testInt", 0, NULL));
    /* 读取node testFloat的值,打印结果为testFloat=0.500000 */
    printf("testFloat=%lf\n", fs_Config_node_float_get_first(pConfig, pConfig, pConfig, "node testFloat", 0, NULL));
    pObjectBase->_delete(pObjectBase);
    fs_Config_delete__OI(pConfig, NULL);
    getchar();
    return EXIT_SUCCESS;
}
///////////////////////////定义公共函数结束//////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#endif