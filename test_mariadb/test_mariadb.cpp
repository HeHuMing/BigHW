/* --
    1、将附件给出的 include_mariadb_x86.rar 文件解压到 BigHW目录下
        目录结构：BigHW -> include_mariadb_x86 -> mysql -> mysql.h等文件
    2、将附件给出的 lib_mariadb_x86.rar 文件解压到 BigHW目录下
        目录结构：BigHW -> lib_mariadb_x86 -> libmariadb.lib等文件
    3、在BigHW中建立test_mariadb项目，将test_mariadb.cpp放入项目文件中并添加到项目中，再把../lib_mariadb_x86/libmariadb.lib也添加到项目中（mysql.h不需要）
    4、应该可以编译并正确运行
    5、注意事项
        a) 考虑到网络安全因素，必须在校园网环境内，校外需要先VPN登录
        b) 附件给出的lib是x86模式，因此必须保证项目是x86模式
        c) 如果离开集成环境，在cmd下运行，则需要将../lib_mariadb_x86/libmariadb.dll放入exe文件所在目录
*/
#include <iostream>     // cin,cout等
#include <iomanip>      // setw等
#include "../include_mariadb_x86/mysql/mysql.h"      // mysql特有
using namespace std;

const char* dbserver = "10.80.42.244";
const char* dbuser = "hwapp_oop";
const char* dbpasswd = "hwapp_2025*OoP-12*18";
const char* dbname = "homework";

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
 ***************************************************************************/
int main(int argc, char* argv[])
{
    MYSQL* mysql;
    MYSQL_RES* result;
    MYSQL_ROW  row;

    /* 初始化 mysql 变量，失败返回NULL */
    if ((mysql = mysql_init(NULL)) == NULL) {
        cout << "mysql_init failed" << endl;
        return -1;
    }

    /* 连接数据库，失败返回NULL
        1、mysqld没运行
        2、没有指定名称的数据库存在 */
    if (mysql_real_connect(mysql, dbserver, dbuser, dbpasswd, dbname, 0, NULL, 0) == NULL) {
        cout << "mysql_real_connect failed(" << mysql_error(mysql) << ")" << endl;
        return -1;
    }

    /* 设置字符集，否则读出的字符乱码 */
    mysql_set_character_set(mysql, "gbk");

    /* -----------------------------
          进行第一次查询 - Begin
       -----------------------------*/
       /* 查询命令：
          select * from view_student_oop limit 10
          含义：表示查询 view_student_oop 表的所有记录，每条记录按原始顺序显示所有字段
                limit 10 表示取前10条记录，如果去掉，则显示全部记录（后续不再重复说明）
          返回值：成功返回0，不成功非0
          不成功的常见可能性：
            1、查询字符串存在语法错误
            2、查询不存在的数据表 */
    if (mysql_query(mysql, "select * from view_student_for_oop limit 10")) {
        cout << "mysql_query failed(" << mysql_error(mysql) << ")" << endl;
        return -1;
    }

    /* 将查询结果存储起来，出现错误则返回NULL
        注意：查询结果为NULL，不会返回NULL */
    if ((result = mysql_store_result(mysql)) == NULL) {
        cout << "mysql_store_result failed" << endl;
        return -1;
    }

    /* 打印当前查询到的记录的数量 */
    cout << "select return " << (int)mysql_num_rows(result) << " records" << endl;

    /* 循环读取所有满足条件的记录
        1、homework数据库的student表结构及数据如下：
            view_student_oop 表的基本结构如下，共14个字段
            +------------------+------------+------+-----+---------------------+-------+
            | Field            | Type       | Null | Key | Default             | Extra |
            +------------------+------------+------+-----+---------------------+-------+
            | stu_term         | char(11)   | NO   |     |                     |       | 学期
            | stu_grade        | char(4)    | NO   |     |                     |       | 年级
            | stu_no           | char(8)    | NO   |     |                     |       | 学号
            | stu_name         | char(32)   | NO   |     |                     |       | 姓名
            | stu_sex          | varchar(1) | NO   |     |                     |       | 性别
            | stu_school_fname | char(64)   | NO   |     |                     |       | 学院全称
            | stu_school_sname | char(16)   | NO   |     |                     |       | 学院简称
            | stu_class_fname  | char(64)   | NO   |     |                     |       | 专业全称
            | stu_class_sname  | char(16)   | NO   |     |                     |       | 专业简称
            | stu_userlevel    | char(1)    | NO   |     |                     |       | 用户级别
            | stu_enable       | varchar(1) | NO   |     |                     |       | 是否允许登录
            | stu_add_date     | datetime   | NO   |     | 0000-00-00 00:00:00 |       | 加入系统时间
            | stu_cno          | char(16)   | YES  |     | NULL                |       | 课号
            | stu_cno_is_del   | varchar(1) | NO   |     |                     |       | 是否退课
            +------------------+------------+------+-----+---------------------+-------+
        2、mysql_fetch_row(result)返回的row是一个char型指针数组，元素个数为查询结果的字段数+1，本例中，row[0]~row[13]有内容，row[14]为NULL
        3、row的排列顺序与select返回的列顺序相同，本例：
            row[0]  <=> stu_term
            ...
            row[13] <=> stu_cno_is_del
            row[14] <=> NULL
        4、不论数据库中是什么类型，C中都当作是字符串来进行处理，如果有必要，需要自己进行转换
        5、根据自己的需要组织输出格式，比如加边框线等 */
    while ((row = mysql_fetch_row(result)) != NULL) {
        cout << setiosflags(ios::left);           //输出左对齐
        /* 共13项，目前只打印了5项 */
        cout << "学号：" << setw(8) << row[2];
        cout << "姓名：" << setw(12) << row[3];
        cout << "性别：" << setw(4) << row[4];
        cout << "专业：" << setw(32) << row[7];
        cout << "学院：" << setw(32) << row[5];
        cout << endl;
    }
    cout << endl;

    /* 释放result，否则会丢失内存 */
    mysql_free_result(result);

    /* -----------------------------
          进行第一次查询 - End
       -----------------------------*/

       /* -----------------------------
             进行第二次查询 - Begin
          -----------------------------*/
          /* 查询命令：
              select stu_sex, stu_no, stu_name from view_student_for_oop  where stu_sex = "女" limit 5
              含义：表示查询student表的性别为"女"的所有记录，每条记录要性别、年龄、姓名三个字段且桉此顺序排序
              返回值：成功返回0，不成功非0
              不成功的常见可能性：
                1、查询字符串存在语法错误
                2、查询不存在的数据表 */
    if (mysql_query(mysql, "select stu_sex, stu_no, stu_name from view_student_for_oop  where stu_sex = \"女\" limit 5")) {
        cout << "mysql_query failed(" << mysql_error(mysql) << ")" << endl;
        return -1;
    }

    /* 将查询结果存储起来，出现错误则返回NULL
        注意：查询结果为NULL，不会返回NULL */
    if ((result = mysql_store_result(mysql)) == NULL) {
        cout << "mysql_store_result failed" << endl;
        return -1;
    }

    /* 打印当前查询到的记录的数量 */
    cout << "select return " << (int)mysql_num_rows(result) << " records" << endl;

    /* 循环读取所有满足条件的记录
        2、row的排列顺序与select返回的列顺序相同，本例：
            row[0] <=> stu_sex
            row[1] <=> stu_no
            row[2] <=> stu_name
            row[3] <=> NULL */
    while ((row = mysql_fetch_row(result)) != NULL) {
        cout << setiosflags(ios::left);           //输出左对齐
        cout << "性别：" << setw(4) << row[0];     //宽度4位，左对齐
        cout << "学号：" << setw(8) << row[1];     //    4
        cout << "姓名：" << setw(12) << row[2];     //    8
        cout << endl;
    }
    cout << endl;

    /* 释放result，否则会丢失内存 */
    mysql_free_result(result);
    /* -----------------------------
          进行第二次查询 - End
       -----------------------------*/

       /* -----------------------------
             进行第三次查询 - Begin
          -----------------------------*/
          /* 查询命令：
              select stu_sex, stu_no, stu_name, stu_class_fname from view_student_for_oop  where stu_sex = "女" and stu_class_sname = "信安" limit 3
              含义：表示查询student表的性别为"女"并且专业简称为"信安"的记录，每条记录要学号、姓名、年龄、专业全称四个字段且桉此顺序排序
              返回值：成功返回0，不成功非0
              不成功的常见可能性：
                1、查询字符串存在语法错误
                2、查询不存在的数据表 */
    if (mysql_query(mysql, "select stu_sex, stu_no, stu_name, stu_class_fname from view_student_for_oop  where stu_sex = \"女\" and stu_class_sname = \"信安\" limit 3")) {
        cout << "mysql_query failed(" << mysql_error(mysql) << ")" << endl;
        return -1;
    }

    /* 将查询结果存储起来，出现错误则返回NULL
        注意：查询结果为NULL，不会返回NULL */
    if ((result = mysql_store_result(mysql)) == NULL) {
        cout << "mysql_store_result failed" << endl;
        return -1;
    }

    /* 打印当前查询到的记录的数量 */
    cout << "select return " << (int)mysql_num_rows(result) << " records" << endl;

    /* 循环读取所有满足条件的记录
        2、row的排列顺序与select返回的列顺序相同，本例：
            row[0] <=> ssex
            row[1] <=> sage
            row[2] <=> sname
            row[3] <=> sdept
            row[4] <=> NULL      */
    while ((row = mysql_fetch_row(result)) != NULL) {
        cout << setiosflags(ios::left);           //输出左对齐
        cout << "性别：" << setw(4) << row[0];     //宽度4位，左对齐
        cout << "学号：" << setw(8) << row[1];     //    4
        cout << "姓名：" << setw(12) << row[2];     //    8
        cout << "专业全称：" << setw(32) << row[3];     //    4
        cout << endl;
    }
    cout << endl;

    /* 释放result，否则会丢失内存 */

    mysql_free_result(result);
    /* -----------------------------
          进行第三次查询 - End
       -----------------------------*/

       /* -----------------------------
             进行第四次查询 - begin
          -----------------------------*/
          /* 查询命令：
             select * from view_student_for_oop where stu_school_sname = "交通" or stu_school_sname = "汽车" order by stu_add_date, stu_no
             含义：表示查询student表的学院简称为"交通"或"汽车"的所有记录，按加入系统的时间排序，时间相同的按学号排序
             返回值：成功返回0，不成功非0
             不成功的常见可能性：
               1、查询字符串存在语法错误
               2、查询不存在的数据表 */
    if (mysql_query(mysql, "select * from view_student_for_oop where stu_school_sname = \"交通\" or stu_school_sname = \"汽车\" order by stu_add_date, stu_no")) {
        cout << "mysql_query failed(" << mysql_error(mysql) << ")" << endl;
        return -1;
    }

    /* 将查询结果存储起来，出现错误则返回NULL
        注意：查询结果为NULL，不会返回NULL */
    if ((result = mysql_store_result(mysql)) == NULL) {
        cout << "mysql_store_result failed" << endl;
        return -1;
    }

    /* 打印当前查询到的记录的数量 */
    cout << "select return " << (int)mysql_num_rows(result) << " records" << endl;

    /* 循环读取所有满足条件的记录 */
    while ((row = mysql_fetch_row(result)) != NULL) {
        cout << setiosflags(ios::left);           //输出左对齐
        cout << "学号：" << setw(8) << row[2];    //宽度12位，左对齐
        cout << "姓名：" << setw(12) << row[3];
        cout << "性别：" << setw(4) << row[4];
        cout << "学院简称：" << setw(16) << row[5];
        cout << "加入时间：" << setw(20) << row[11];
        cout << endl;
    }
    cout << endl;

    /* 释放result，否则会丢失内存 */
    mysql_free_result(result);
    /* -----------------------------
          进行第四次查询 - End
       -----------------------------*/

       /* 关闭整个连接 */
    mysql_close(mysql);

    return 0;
}