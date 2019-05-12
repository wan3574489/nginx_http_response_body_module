

#include "ngx_http_response_body.h";
#include <ngx_string.h>
static ngx_http_output_body_filter_pt    ngx_http_next_body_filter;

static ngx_str_t new_varaible_response_body = ngx_string("response_body");
static ngx_int_t new_varaible_response_body_index;

static ngx_command_t ngx_http_response_body_command[] = {
        ngx_null_command
};

static ngx_http_module_t ngx_http_response_body_ctx = {
        ngx_http_response_body_preconfiguration,    /* preconfiguration (4)*/
        ngx_http_response_body_ini,                                       /* postconfiguration (8) */

        NULL,    /* create main configuration (1) */
        NULL,      /* init main configuration (5) */

        NULL,   /* create server configuration  (2) */
        NULL,    /* merge server configuration (6) */

        NULL,   /* create location configuration (3) */
        NULL     /* merge location configuration (7) */
};

ngx_module_t ngx_http_response_body_module = {
        NGX_MODULE_V1,
        &ngx_http_response_body_ctx,
        ngx_http_response_body_command,
        NGX_HTTP_MODULE,                       /* module type */
        NULL,                                  /* init master */
        NULL,                                  /* init module */
        NULL,                                  /* init process */
        NULL,                                  /* init thread */
        NULL,                                  /* exit thread */
        NULL,                                  /* exit process */
        NULL,                                  /* exit master */
        NGX_MODULE_V1_PADDING
};


static ngx_int_t   ngx_http_response_body_preconfiguration(ngx_conf_t *cf){

    ngx_http_variable_t *v;
    v = ngx_http_add_variable(cf,&new_varaible_response_body,NGX_HTTP_VAR_CHANGEABLE);
    if (v == NULL){
        return NGX_ERROR;
    }
    v->get_handler = ngx_http_variable_response_body_get_handler;
    v->data = 0;

    return NGX_OK;
}

static ngx_int_t ngx_http_response_body_ini(ngx_conf_t *cf){

    ngx_http_next_body_filter = ngx_http_top_body_filter;
    ngx_http_top_body_filter = ngx_http_response_body_filter;

    new_varaible_response_body_index = ngx_http_get_variable_index(cf,&new_varaible_response_body);

    return NGX_OK;
}

static ngx_int_t ngx_http_response_body_filter(ngx_http_request_t *r, ngx_chain_t *chain){
    int i = 0;
    ngx_chain_t  *cl;
    ngx_buf_t    *buf, *next;
    size_t        len;
    u_char       *p;

    ngx_http_variable_value_t *v = ngx_http_get_indexed_variable(r,new_varaible_response_body_index);
    //
    cl = chain;
    buf = cl->buf;

    if (cl->next == NULL) {
        v->len = buf->last - buf->pos;
        v->valid = 1;
        v->no_cacheable = 0;
        v->not_found = 0;
        v->data = buf->pos;

        return NGX_OK;
    }

    next = cl->next->buf;
    len = (buf->last - buf->pos) + (next->last - next->pos);

    p = ngx_pnalloc(r->pool, len);
    if (p == NULL) {
        return NGX_ERROR;
    }

    v->data = p;

    p = ngx_cpymem(p, buf->pos, buf->last - buf->pos);
    ngx_memcpy(p, next->pos, next->last - next->pos);

    v->len = len;
    v->valid = 1;
    v->no_cacheable = 0;
    v->not_found = 0;


    return ngx_http_next_body_filter(r,chain);
}

static  ngx_int_t ngx_http_variable_response_body_get_handler(ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data){

    v->not_found = 1;

    return NGX_OK;
}