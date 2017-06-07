package usacreport;

import java.io.*;

/**
 * Description: 报表工具
 * Company: daqo
 * 数据库接口
 * version 1.0
 */

public class dllf1j
    implements Serializable {
   private static final dllf1j INSTANCE = new dllf1j();
   public static dllf1j getInstance() {
      return INSTANCE;
   }

   public dllf1j() {
   }

   public native int startdb(); //连接数据库

   public native int closedb();

   public native byte[] getenvvar(); //得到环境变量

   public native int getTableInfo(byte[] dbname, byte[] tablename,
                                  byte[][] tableinfo, int[] maxlength,
                                  byte[] tableType);

   public native int openTableInfo(byte[] dbname, byte[] tablename);

   public native void closeTableInfo();

   public native int readdata(byte[][] buf);

   public native int readRecorders(byte[] dbname, byte[] tablename,
                                   byte[] fieldname, byte[] where,
                                   byte[] orderfn, int[] fieldcount,
                                   boolean bDistinct);
   public native byte[] readRtdbRecorders( byte[] tablename,
                                byte[] name,byte[] field);

   public native int insertARecord(byte[][] colvalue);

   public native int updateARecord(byte[][] colvalue, int[] field,
                                   int keycolnums);

   public native int removeARecord(byte[] where);

   public native int reCall(byte[][] server, byte[][] name); //加载实时库

   public native int reloadRtdb(byte[][] server); //加载实时库

   public native int reloadMode(int load_type);

   public native int sendMsgToFert(int groupno, byte type, byte[] tablename);

   public native byte[] makeBcp(byte[] dbname, byte[] srvname, byte[] filename,
                                byte[] username, byte[] pass, boolean inoutf); //生成bcp文件

   public native int NetFileSave(byte[] filename, byte[] buf, int len, int flag);

   public native int ExecuteCmd(byte[] dbname, byte[] sql);

   public native double SqlReadRecorder(byte[] dbname, byte[] sql, int type);

   public native int SqlReadRecorders(byte[] dbname,byte[] sql,int[] fieldcount);

   //public native int SqlReadRecorders(byte []dbname,byte []sql,byte [][] buf);
   public native byte[] RString();

   public native void WString(byte[] name);

   static {
      try {
         System.loadLibrary("dbodbc"); //f1jdll//dlllink
//      System.out.println("f1jdll连接成功");
      }
      catch (SecurityException se) {
         System.out.println("1" + se.getMessage());
      }
      catch (UnsatisfiedLinkError ule) {
         System.out.println("2" + ule.getMessage());
      }
   }
}
