
# 小雉配置

#### 基于“小雉视频系统”中的配置组件构建
github   http://www.feitianzhi.com/  
gitee   http://www.feitianzhi.com/  
 
#### **介绍**
小雉配置简介

“小雉配置”为解决“小雉视频系统”项目中遇到的实际问题而开发的通用配置模块,旨在实现一种模块化配置,支持配置字段任意增删嵌套也能保持配置前后兼容的算法,满足“小雉视频系统”的未知开发需求(后期要增加的功能,ai算法是未知的,功能及ai算法需要的参数未知,有更优秀算法时已有算法需要被移除,其对应参数也应被删除)。

QQ交流群：869598376  
github   https://github.com/feitianzhi/fslib-config  
gitee   https://gitee.com/feitianzhi/fslib-config  

#### **需求分析**
------
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;软件的开发离不开配置，传统的软件设计包括前端、后台和数据库3部分，三者是密切配合的统一整体，在实际项目中往往遇到以下问题：

 1. 因项目需求不明而增加、修改、删除参数导致配置结构调整后难以同已有数据兼容；
 2. 后台参数修改后，前端需要同步修改，无法做到老版本前端与新版本后台配合（新版本后台可能修正了bug,老版本前端属于老项目），修正老项目bug需要在老版本分支上进行，代码分支多，维护困难；
 3. 配置备份困难。首先后台与数据库之间有多个访问渠道，配置备份需要停机备份；其次备份的配置无法线上热还原（有差异部分相关模块重置，无修改部分持续工作，如仅1相机参数有区别，仅停掉1相机进行重连，其余相机持续工作）；最后备份的配置无法在新版本或老版本上进行还原（因为字段结构不一样）；
 4. 授权困难，配置臃肿。为实现所有用户需求，软件的参数量非常庞大，而对一个具体项目可能只需要其中的很少只关联10个参数的功能，把大量的参数置于系统中会拖慢系统（一个表只有1列可以允许插入1亿行，但如有1000列，可能表就只能插入10万行了）同时软件中需要通过授权判断而限制用户在授权范围内操作，系统调试困难--授权是在做一个减法操作；


#### **思考与假设**
------
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;“小雉视频系统”是一套集所有客户需求于一体的一套视频软件，参数的修改随客户的要求变化而变化，均为不可预知的参数需求，为每位客户单独开一个分支会增加bug修正的同步成本和升级包制作成本，“小雉视频系统”迫切需要一套廉价的高效率的配置设计方案，要求方案具备如下特性：

 1. 配置项在各个模块中申明（限制值的类型、范围、个数和有效条件（比如A参数必须要B参数为1时才有效可设置）），在模块中读取，把参数的管理放到各个模块中，各个模块只需要根据本模块的需要申明参数，实现参数的模块化（移除此模块也移除了此模块的参数）；
 2. 各个模块申明配置项时如同全新开发一样申明参数即可，同历史参数的兼容归配置模块算法实现；
 3. 各个模块如果未被授权，则不申明参数，通过减少参数增加系统的容量及并发，既功能少，并发高，让授权是一个加法操作；
 4. 申明参数时指定默认值，在客户端传参时未有对应参数时读取此参数时读到的值为默认值，以此实现对历史客户端的兼容（历史客户端与新版本后台字段有差异部分使用默认值代替，保证新后台可同老客户端适配）;
 5. 申明参数时同时申明中文名、注释、可选值，即把注释写入配置，在配置导出时可自动对参数注释，实现文档源码化;
 6. 配置可以热导出，导出后的配置可热导入到任意版本的后台，配置模块可自动探测到变化，自动完成对象实例的增删改动作；
 7. 配置可以导出xml,json等格式，并支持xml,json的导入；
 8. 配置可导出非默认值部分字段，也能导入，实现瘦客户端编程（服务器可以集所有功能与一体，但针对具体的项目客户，可能只需要部分功能，可在完整客户端上正确配置对应项目需求的功能参数后导出参数，瘦客户端上只针对导出参数进行开发，减少参数可提高客户端的人性化程度）；


#### **小雉配置解决方案**
------
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;基于假设“小雉配置”采用面向属性的配置设计方式，把配置项分为节点型、模板、字符串、整型、浮点和二进制共6种数据类型，每种数据类型可设置多个条件组，以条件组内的所有条件为真此条件组为真，任何一个条件组为真则参数有效；
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;“小雉配置”采用C语言编写，可用于windows,linux,arm等平台，配置为单文件，本地可拷贝备份，远程可热导入导出xml和json,配置可承载数千参数（结构可类似xml任意层级嵌套）数十万级别的量（类比一张表有上千列，表可以容纳数十万行）；

#### **小雉配置数据类型**
------
1. 节点型类型
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;类似于xml中的a->b->c,其中b节点包含c,则b为节点型类型，且a下面有且只能有一个b；节点型类型可以作为节点型节点和模板节点的子类型，可以设置中文名、注释、条件判断，不能设置默认值，可选值；
    fs_Config_node_node_add //添加节点型参数
    fs_Config_node_get_first //获取节点型参数
2. 模板类型
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;类似于xml中的a->b->c,其中b节点包含c,如b固定且只有一个则b为节点型类型，如b可以是0到多个，则b为模板类型，申明模板实质是申明一个类，配置时在a下创建此类的多个实例（创建多少个实例，a下有多少个b,b的子节点与类的结构完全相同）；模板类型主要用于同类型数据的管理（比如添加相机）；模板类型可以作为节点型节点和模板节点的子类型，可以设置中文名、注释、条件判断、可创建实例的个数、时间控制参数（比如有两个模板实例，第一个模板实例在0-7点生效，另外一个在7-24时生效，实现参数随时间变化的控制），不能设置默认值，可选值；
    fs_Config_node_template_add //添加节点型参数
    fs_Config_node_template__IO //获取节点型参数
3. 字符串类型
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;字符串类型是一个储存字符串数据的节点类型；字符串类型可以作为节点型节点和模板节点的子类型，可以设置中文名、注释、条件判断、可设置值的个数、字符串长度、默认值、可选值；
    fs_Config_node_string_add //添加字符串类型参数
    fs_Config_node_string_get_first //获取字符串类型参数
4. 整型类型
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;整型类型是一个储存64位有符号整数的节点类型；整型类型可以作为节点型节点和模板节点的子类型，可以设置中文名、注释、条件判断、可设置值的个数、值的范围、默认值、可选值；
    fs_Config_node_integer_add //添加整型类型参数
    fs_Config_node_integer_get_first //获取整型类型参数
5. 浮点类型
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;浮点类型是一个储存64位浮点的节点类型；浮点类型可以作为节点型节点和模板节点的子类型，可以设置中文名、注释、条件判断、可设置值的个数、值的范围、默认值、可选值；
    fs_Config_node_float_add //添加浮点类型参数
    fs_Config_node_float_get_first //获取浮点类型参数
6. 二进制类型
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;二进制类型是一个储存任意数据类型的节点类型；二进制类型可以作为节点型节点和模板节点的子类型，可以设置中文名、注释、条件判断、可设置值的个数、数据长度范围、默认值、可选值；
    fs_Config_node_binary_add //添加二进制类型参数
    fs_Config_node_binary_get_first //获取二进制类型参数

#### **小雉配置的简单演示**
------

    int main() {
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
    return 0;
    }
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;代码生成了"test.cfg"和"test1.cfg"两份配置文件,可在[http://www.feitianzhi.com/boke/index.php/ziyuanxiazai.html][1]页面下载"小雉系统工具"打开配置文件,该项目上传的"含有上千字段的综合配置演示实例.cfg"为"小雉视频系统"使用该配置模块创建的配置文件,也可以使用"小雉系统工具"打开编辑；

  [1]: http://www.feitianzhi.com/boke/index.php/ziyuanxiazai.html