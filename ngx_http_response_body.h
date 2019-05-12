//
// Created by wanzengchuang on 19-5-9.
//

#ifndef NGINX_NGX_HTTP_REQUEST_BODY_H
#define NGINX_NGX_HTTP_REQUEST_BODY_H

#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include <time.h>

static ngx_int_t ngx_http_response_body_preconfiguration(ngx_conf_t *cf);
static ngx_int_t ngx_http_response_body_ini(ngx_conf_t *cf);
static ngx_int_t ngx_http_response_body_filter(ngx_http_request_t *r, ngx_chain_t *chain);
static ngx_int_t ngx_http_variable_response_body_get_handler(ngx_http_request_t *r,
                                                              ngx_http_variable_value_t *v, uintptr_t data);
#endif //NGINX_NGX_HTTP_REQUEST_BODY_H
