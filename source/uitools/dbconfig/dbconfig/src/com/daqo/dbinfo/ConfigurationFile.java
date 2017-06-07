package com.daqo.dbinfo;

/**
 * Description: 数据库维护工具
 * Company: daqo
 * version 1.0
 */

//洪祥  java读取ini文件的类
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public final class ConfigurationFile {
 /**
  * 从ini配置文件中读取变量的值
  * @param file 配置文件的路径
  * @param section 要获取的变量所在段名称
  * @param variable 要获取的变量名称
  * @param defaultValue 变量名称不存在时的默认值
  * @return 变量的值
  * @throws IOException 抛出文件操作可能出现的io异常
  */
 public static String getProfileString(
  String file,
  String section,
  String variable,
  String defaultValue)
  throws IOException {
  String strLine, value = "";
  BufferedReader bufferedReader = new BufferedReader(new FileReader(file));
  boolean isInSection = false;
  try {
   while ((strLine = bufferedReader.readLine()) != null) {
    strLine = strLine.trim();
    strLine = strLine.split("[;]")[0];
    Pattern p;
    Matcher m;
    p = Pattern.compile("\\[.*\\]");
    m = p.matcher((strLine));
    if (m.matches()) {
     p = Pattern.compile("\\[" + section + "\\]");
     m = p.matcher(strLine);
     if (m.matches()) {
      isInSection = true;
     } else {
      isInSection = false;
     }
    }
    if (isInSection == true) {
     strLine = strLine.trim();
     String[] strArray = strLine.split("=");
     if (strArray.length == 1) {
      value = strArray[0].trim();
      if (value.equalsIgnoreCase(variable)) {
       value = "";
       return value;
      }
     } else if (strArray.length == 2) {
      value = strArray[0].trim();
      if (value.equalsIgnoreCase(variable)) {
       value = strArray[1].trim();
       return value;
      }
     } else if (strArray.length > 2) {
      value = strArray[0].trim();
      if (value.equalsIgnoreCase(variable)) {
       value = strLine.substring(strLine.indexOf("=") + 1).trim();
       return value;
      }
     }
    }
   }
  } finally {
   bufferedReader.close();
  }
  return defaultValue;
 }
}
