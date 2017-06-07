package com.daqo.dbinfo;

/**
 * Description: 数据库维护工具
 * Company: daqo
 * version 1.0
 */

//服务类型参数表
public class ServiceTypeTable {

  String serviceType; //服务类型
  String serviceDescrible; //服务描述
  String ParaTablename; //参数库名
  boolean isVisible;
  public ServiceTypeTable() {
    serviceType = "";
    serviceDescrible = "";
    ParaTablename = "";
    isVisible = false;
  }
}
