package com.daqo.dbinfo;

import java.util.Vector;
import java.util.ArrayList;

/**
 * Description: 数据库维护工具
 * Company: daqo
 * version 1.0
 */

public class ManageModel {

  private Vector rowChangFlag = new Vector();//每行修改标志
  private Vector vHideField = new Vector(0);//记录没有显示的域

  public ManageModel() {
  }

  //初始化标志为没有修改
  public void initFlag(int rowcount)
      {
        for(int i = 0; i< rowcount; i++)
          {
            rowChangFlag.add(new Integer(ConstantDef.DB_UNCHANGED )) ;
          }
      }

      //增加一条记录的插入标准
  public void addArec()
      {
        rowChangFlag.add(new Integer(ConstantDef.DB_INSERT  )) ;
      }

//恢复一条记录的标志
      public void unRemoveArec()
      {
        rowChangFlag.add(new Integer(ConstantDef.DB_UNCHANGED  )) ;
      }

      public void unRemoveRecs(int nums)
          {
            for(int i = 0; i<nums; i++)
              {
                rowChangFlag.add(new Integer(ConstantDef.DB_UNCHANGED  )) ;
              }
          }

//插入一条记录的标志
      public void insertArec(int row)
          {
            rowChangFlag.insertElementAt(new Integer(ConstantDef.DB_INSERT) , row);
          }
//删除一条记录的标记
      public void removeArec( int row)
          {
            rowChangFlag.removeElementAt(row) ;
          }

//一条记录修改了，把标志做相应的修改
      public void updateArec(int row)
          {
            rowChangFlag.setElementAt(new Integer(ConstantDef.DB_CHANGED ),row) ;
          }

//设置一条记录的标志为没有修改过
      public void setUnDoFlag(int row)
          {
            rowChangFlag.setElementAt(new Integer(ConstantDef.DB_UNCHANGED ),row) ;
          }

//设置代码校验错误标志
     public void setCodeErr(int row)
         {
           rowChangFlag.setElementAt(new Integer(ConstantDef.DB_CODE_ERR  ),row) ;
         }

//排序
      public void changeOrder(int[]index)
          {
           Vector rowChangFlag1 = (Vector) rowChangFlag.clone() ;
           rowChangFlag.setSize(0) ;
           for(int i = 0; i < index.length ;i++)
             {
               rowChangFlag.add(rowChangFlag1.elementAt(index[i]) ) ;

             }
          }
//得到某条记录的标志
    public int getRowFlag(int row)
        {
          return  Integer.parseInt(rowChangFlag.elementAt(row) .toString() );
        }


/*******************************/
/*
    开始不显示域的处理
*********************************/
public void addAField(String fieldname)
    {
      vHideField.add(fieldname) ;
    }

public int getHideCount()
    {
      return vHideField.size() ;
    }

    public String getAFieldname( int col)
        {
          if(col >= getHideCount()) return "";
          return vHideField.elementAt(col) .toString() ;
        }

        //判断一个字段是否隐藏
        //
  public boolean isHideFiled(String fieldname)
      {
        for(int i = 0; i<vHideField.size() ;i++)
          {
            if(fieldname.equalsIgnoreCase( vHideField.elementAt(i).toString()  ) )
              return true;
          }
          return false;
      }


  /*******************************/
  /*
      结束不显示域的处理
  *********************************/


}
