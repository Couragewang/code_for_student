#pragma once

#include <iostream>

typedef struct Request{
    int x;
    int y;
    int op; //+-*/%->01234
}Request_t;

typedef struct Response{
    int result;
    int status;// 0, 1, 2
}Response_t;
