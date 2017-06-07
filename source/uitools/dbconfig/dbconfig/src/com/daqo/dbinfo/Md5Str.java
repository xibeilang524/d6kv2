package com.daqo.dbinfo;
import java.security.*;

/**
 * Description: 数据库维护工具
 * Company: daqo
 * version 1.0
 */

//add by hx
public class Md5Str {

  public String PWDtoMD5(String strpwd) {

    StringBuffer sb = new StringBuffer(32);
    try {
      MessageDigest md5 = MessageDigest.getInstance("MD5");
      byte[] md5result = md5.digest(strpwd.getBytes());
      for (int i = 0; i < md5result.length; i++) {
        sb.append(Integer.toHexString( (md5result[i] & 0xFF) | 0x100).toUpperCase().substring(1, 3));
      }
    } catch (NoSuchAlgorithmException ex) {
      System.out.print(ex.toString());
    }
    return sb.substring(0,20).toString();
  }
 }


