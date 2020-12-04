/*
 * @Author: gongluck 
 * @Date: 2020-05-17 16:00:50 
 * @Last Modified by: gongluck
 * @Last Modified time: 2020-05-17 18:21:40
 */

#ifndef __MYFUN_H__
#define __MYFUN_H__

#if(defined myfun_EXPORTS) && (defined WIN32)
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT
#endif

DLL_EXPORT int myfun(int a, int b);

#endif//__MYFUN_H__
