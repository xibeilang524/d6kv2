package com.daqo.dbinfo;

/**
 * Description: 数据库维护工具
 * Company: daqo
 * version 1.0
 */

//区域参数
public class StationTable {

  String StationName; //站代码
  String StationDescrible; //站名
  int    StationAreaID;
  public StationTable() {
    StationAreaID = 0;
    StationDescrible = "";
    StationName="";
  }
}
