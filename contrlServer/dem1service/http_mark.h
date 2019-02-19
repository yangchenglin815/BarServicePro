#ifndef __HTTP_MARK_H__  
#define __HTTP_MARK_H__  


#define HTTP_SUCCESS       (0)          // 操作成功  
#define HTTP_ERROR_UNKNOWN      (-1)         // 未知的错误  
#define HTTP_ERROR_NETWORK      (-2)         // 网络连接失败  
#define HTTP_ERROR_HTTP_HEAD    (-3)         // 未找到协议头 http || https   

#define HTTP_ERROR_SERVICE      (-1000)      // 服务器请求失败  
#define HTTP_ERROR_LOGIN        (-1001)      // 登录失败  
#define HTTP_ERROR_ID           (-1002)      // 企业ID错误  
#define HTTP_ERROR_USER         (-1003)      // 帐号不存在  
#define HTTP_ERROR_PASSWORD     (-1004)      // 密码错误  



#endif // __HTTP_MARK_H__  