/*
 * @Author: gongluck 
 * @Date: 2020-12-08 19:10:29 
 * @Last Modified by: gongluck
 * @Last Modified time: 2020-12-08 19:11:06
 */

// ./configure --add-module=/mnt/e/Code/CVIP/code/nginx/module
// sudo ./objs/nginx -c /mnt/e/Code/CVIP/code/nginx/module/hello.conf

#include <ngx_http.h>
#include <ngx_config.h>
#include <ngx_core.h>

static char *ngx_http_hello_module_set(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
static ngx_int_t ngx_http_hello_module_handler(ngx_http_request_t *r);

// 模块配置结构
typedef struct
{
    ngx_str_t hello_string;
} ngx_http_hello_loc_conf_t;

// 模块配置指令
static ngx_command_t hello_commands[] = {
    {
        ngx_string("hello"),                 //配置指令的名称
        NGX_HTTP_LOC_CONF | NGX_CONF_NOARGS, //该配置指令属性的集合
        ngx_http_hello_module_set,           //当nginx在解析配置的时候，如果遇到这个配置指令，将会把读取到的值传递给这个函数进行分解处理
        NGX_HTTP_LOC_CONF_OFFSET,            //指定当前配置项存储的内存位置,实际上是使用哪个内存池的问题。
        0,                                   //指定该配置项值的精确存放位置，一般指定为某一个结构体变量的字段偏移。
        NULL                                 //可以指向任何一个在读取配置过程中需要的数据，以便于进行配置读取的处理。
    },
    ngx_null_command};

// 模块上下文结构
static ngx_http_module_t hello_ctx = {
    NULL, //在创建和读取该模块的配置信息之前被调用
    NULL, //在创建和读取该模块的配置信息之后被调用

    NULL, //调用该函数创建本模块位于http block的配置信息存储结构。该函数成功的时候，返回创建的配置对象。失败的话，返回NULL。
    NULL, //调用该函数初始化本模块位于http block 的配置信息存储结构。该函数成功的时候，返回NGX_CONF_OK。失败的话，返回NGX_CONF_ERROR或错误字符串。

    NULL, //调用该函数创建本模块位于http server block的配置信息存储结构，每个server block会创建一个。该函数成功的时候，返回创建的配置对象。失败的话，返回NULL。
    NULL, //因为有些配置指令既可以出现在http block，也可以出现在http server block中。那么遇到这种情况，每个server都会有自己存储结构来存储该server的配置，但是在这种情况下http block中的配置与server block中的配置信息发生冲突的时候，就需要调用此函数进行合并，该函数并非必须提供，当预计到绝对不会发生需要合并的情况的时候，就无需提供。当然为了安全起见还是建议提供。该函数执行成功的时候，返回NGX_CONF_OK。失败的话，返回NGX_CONF_ERROR或错误字符串。

    NULL, //调用该函数创建本模块位于location block的配置信息存储结构。每个在配置中指明的location创建一个。该函数执行成功，返回创建的配置对象。失败的话，返回NULL。
    NULL, //与merge_srv_conf类似，这个也是进行配置值合并的地方。该函数成功的时候，返回NGX_CONF_OK。失败的话，返回NGX_CONF_ERROR或错误字符串。
};

// ngx_http_hello_module
ngx_module_t ngx_http_hello_module = {
    NGX_MODULE_V1,
    &hello_ctx,      /* module context */
    hello_commands,  /* module directives */
    NGX_HTTP_MODULE, /* module type */
    NULL,            /* init master */
    NULL,            /* init module */
    NULL,            /* init process */
    NULL,            /* init thread */
    NULL,            /* exit thread */
    NULL,            /* exit process */
    NULL,            /* exit master */
    NGX_MODULE_V1_PADDING};

static char *ngx_http_hello_module_set(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ngx_http_core_loc_conf_t *corecf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
    corecf->handler = ngx_http_hello_module_handler;

    return NGX_CONF_OK;
}

static ngx_int_t ngx_http_hello_module_handler(ngx_http_request_t *r)
{
    ngx_str_t str = ngx_string("Hello, Nginx!");
    ngx_buf_t *b = ngx_pcalloc(r->pool, sizeof(ngx_buf_t));
    if (b == NULL)
    {
        return NGX_HTTP_INTERNAL_SERVER_ERROR;
    }

    ngx_chain_t out;
    out.buf = b;
    out.next = NULL;
    b->pos = str.data;
    b->last = b->pos + str.len;
    b->memory = 1;   /* this buffer is in memory */
    b->last_buf = 1; /* this is the last buffer in the buffer chain */

    /* set the status line */
    r->headers_out.status = NGX_HTTP_OK;
    r->headers_out.content_length_n = str.len;

    /* send the headers of your response */
    ngx_int_t rc = ngx_http_send_header(r);
    if (rc == NGX_ERROR || rc > NGX_OK || r->header_only)
    {
        return rc;
    }

    /* send the buffer chain of your response */
    return ngx_http_output_filter(r, &out);
}