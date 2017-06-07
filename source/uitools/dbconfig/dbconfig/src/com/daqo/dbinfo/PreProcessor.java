package com.daqo.dbinfo;

import java.util.logging.Logger;
import java.util.logging.Level;
import java.util.logging.ConsoleHandler;

/**
 * Description: 数据库维护工具
 * Company: daqo
 * version 1.0
 */

//用于版本控制的类，及预处理常数定义
public class PreProcessor
    extends Object {
  public final static boolean XINJIANG = true; //新疆,佳木斯的特定版本,遥信特殊设备编码
  public final static boolean JINHUA   = false; //金华调度特定版本
  public final static boolean ALIAS    = false;  //系统是否使用具有别名的表来显示树节点信息
  public  static boolean MD5 = false ;      //系统的用户密码是否进行MD5的加密验证
  public  static boolean TableLock =false;  //系统对表修改时是否锁定的标志
  public static String vesion ="1.1";    //程序版本号

  final static Logger log = Logger.getLogger("gem");
  public PreProcessor(){
    log.setLevel(Level.FINER);
    ConsoleHandler c1 = new ConsoleHandler();
    c1.setLevel(Level.FINER);
    log.addHandler(c1);
  }
}
