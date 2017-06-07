package com.daqo.dbinfo.table;

import java.util.Vector;

/**
 * Description: 数据库维护工具
 * Company: daqo
 * version 1.0
 */

/*通道参数表，终端参数表，遥信参数表，N4F配置参数表，SCN配置参数表，通信组表，
 通讯参数表，afert.h,后台遥控命令,   void *buf[0]=标示，+"表名"*/
public class FertTable {

  private Vector tablename = new Vector(0);
  private byte[] type = new byte[8];
  public FertTable() {
    tablename.add("遥信参数表"); //0
    tablename.add("终端参数表");
    tablename.add("通道参数表");
    tablename.add("SCI配置表");
    tablename.add("N4F配置参数表");
    tablename.add("转发通讯参数表"); //5
    tablename.add("双遥控号对照参数表");
    tablename.add("遥测参数表");
    type[0] = 2;
    type[1] = 10;
    type[2] = 11;
    type[3] = 12;
    type[4] = 13;
    type[5] = 14;
    type[6] = 15;
    type[7] = 16;
  }

  public byte getType(String tb) {
    for (int i = 0; i < type.length; i++) {
      if (tb.equalsIgnoreCase(tablename.elementAt(i).toString().trim())) {
        return type[i];
      }
    }

    return -1;
  }
}
