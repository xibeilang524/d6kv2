package com.daqo.dbinfo;

/**
 * Description: 数据库维护工具
 * Company: daqo
 * version 1.0
 */
//遥信参数表中，遥控号不允许重复；
//虚拟遥信量(计算量标志＝1)的遥控号可以重复。

public class YKInfo {
  private  MyTableModel  YKNoTableModel;
  private   MyTableModel  YKFlagTableModel;
  private   MyTableModel  RTUTableModel;
  private MyTableModel CALCFlagTableModel;
  private int ykflagcol = -1 , yknocol = -1 , rtucol = -1 ,calcflagcol = -1;
  private String sTableName = ConstantDef.YXPARATABLE;
  private String YKNoName = "遥控序号";
  private String  YKFlagName = "是否遥控";
  private String  RTUName = "终端序号";
  private String curFieldName = "";
  private String CALCFlagName = "计算量标志";

  public YKInfo() {
  }

//是否为遥信参数表
  public boolean isYXTable(String tablename)
  {
    return tablename.equalsIgnoreCase(this.sTableName) ;
  }

//该字段是否为：遥控序号，是否遥控，终端序号,计算量标志
  public boolean isYKSomeField(String fieldname)
  {
    boolean flag =  fieldname.equalsIgnoreCase( this.YKNoName ) ||
                    fieldname.equalsIgnoreCase(this.YKFlagName )  ||
                    fieldname.equalsIgnoreCase(this.RTUName )  ||
                    fieldname.equalsIgnoreCase(this.CALCFlagName ) ;
    if(flag)
    {
      curFieldName = fieldname;
    }
    return flag;
  }

//设置几个关心字段的列号
  public void setYKSomeField(MyTableModel tablemodel)
  {
    for(int i = 0; i<tablemodel.getColumnCount() ;i++)
    {
      setYKSomeField( tablemodel.getColumnName(i)  , tablemodel, i);
    }
  }

//设置几个关心字段的列号
  public void setYKSomeField( String fieldname ,MyTableModel tablemodel,int col)
  {
    if(fieldname.equalsIgnoreCase(this.RTUName  ) )
    {
      setRTU(tablemodel,col);
    }
    else if(fieldname.equalsIgnoreCase(this.YKFlagName ) )
    {
      setYKFlag(tablemodel,col);
    }
    else if(fieldname.equalsIgnoreCase(this.YKNoName ) )
    {
      setYKno(tablemodel,col);
    }
    else if(fieldname.equalsIgnoreCase(this.CALCFlagName ))
    {
      setCALCFlag(tablemodel,col);
    }
  }

  //
  public String getYKNoFieldName()
  {
    return this.YKNoName ;
  }

  public String getYKFlagName ()
  {
    return this.YKFlagName ;
  }
  public String getRTUName()
  {
    return this.RTUName ;
  }

  public String getCALCFlagName()
  {
    return this.CALCFlagName ;
  }

  public void setYKno(MyTableModel yknomodel,int yknocol)
  {
    this.YKNoTableModel = yknomodel;
    this.yknocol = yknocol;
  }
  public void setYKFlag(MyTableModel ykfalgmodel,int ykflagcol)
  {
    this.YKFlagTableModel = ykfalgmodel;
    this.ykflagcol = ykflagcol;
  }

  public void setRTU(MyTableModel rtumodel,int rtucol)
  {
    this.RTUTableModel = rtumodel;
    this.rtucol = rtucol;
  }

  public void setCALCFlag(MyTableModel calcfmodel,int calcflagcol)
  {
    this.CALCFlagTableModel =calcfmodel;
    this.calcflagcol = calcflagcol;
  }

  public  MyTableModel getYKnoModel()
  {
    return YKNoTableModel;
  }

  public int getYKnocol()
  {
    return this.yknocol ;
  }

  public MyTableModel getYKfalgModel()
  {
    return YKFlagTableModel;
  }
  public int getYKFlagcol()
  {
    return ykflagcol;
  }
  public MyTableModel getRTUModel()
  {
    return this.RTUTableModel ;
  }

  public int getRTUcol()
  {
    return this.rtucol ;
  }
  public MyTableModel getCALCFlagModel()
  {
    return this.CALCFlagTableModel ;
  }
  public int getCALCFlagcol()
  {
    return this.calcflagcol ;
  }

  //检测遥控号是否有重复
  //如果有计算量标志(虚遥信)，则不检查遥控号重复
  //return true,表明有重复号.则新的值不接受
  public boolean checkYKNo(Object newValue, int row, int column) {
    boolean checkRet = false;
    //遥控序号,RTU号(终端序号)，是否遥控，这三个字段组合是唯一的

    if (newValue == null) {
      return checkRet;
    }
    String snewyk = newValue.toString().trim();

    //检查该行"是否遥控"字段，若未选或不选，不做检查
    if (getYKFlagName() .equalsIgnoreCase(curFieldName) ) { // 当前是"是否遥控"字段，该记录值由"true"设置成"false",不做检查
      if (!snewyk.equalsIgnoreCase("true")) {
        return checkRet;
      }
    }
    else {
      Object ykflagcolval = YKFlagTableModel.getValueAt(row, ykflagcol); //当前不是编辑"是否遥控"字段,得到"是否遥控"字段的值
      if (ykflagcolval != null) {
        if (!ykflagcolval.toString().trim().equalsIgnoreCase("true")) { //无遥控标志，返回"false"
          return checkRet;
        }
      }
    }

//检查"计算量标志"字段
    if (getCALCFlagName()  .equalsIgnoreCase(curFieldName) ) { // 当前是"计算量标志"字段，该记录值由"false"设置成"true",不做检查
      if (snewyk.equalsIgnoreCase("true")) {
        return checkRet;
      }
    }
    else {
      Object calcfalgcolval = CALCFlagTableModel .getValueAt(row, calcflagcol); //当前不是编辑"计算量标志"字段,得到"计算量标志"字段的值
      if (calcfalgcolval != null) {
        if (calcfalgcolval.toString().trim().equalsIgnoreCase("true")) { //有计算量标志，返回"false"
          return checkRet;
        }
      }
    }

    int rowcount = YKFlagTableModel.getRowCount();
    int ret;
    Object ob;
    for (int i = 0; i < rowcount; i++) {
      if (i == row) {
        continue;
      }
      Object ykflagcolval = YKFlagTableModel.getValueAt(i, ykflagcol);
      if (ykflagcolval == null ||  !ykflagcolval.toString().trim().equalsIgnoreCase("true") ) {//无遥控标志
        continue;
      }
      if ( getYKNoFieldName() .equalsIgnoreCase(curFieldName)) { //遥控序号的值在修改
        if (tmpcmp(i, row, rtucol,RTUTableModel) == 0) {//终端序号相等
          Object yknocolval = YKNoTableModel.getValueAt(i, yknocol);
          if (yknocolval != null) {
            if (yknocolval.toString().trim().equalsIgnoreCase(snewyk)) {
              return!checkRet;
            }
          }
        }
      }
      else if ( getRTUName() .equalsIgnoreCase(curFieldName)) {//终端序号在修改
        if (tmpcmp(i, row, yknocol,YKNoTableModel) == 0) {//遥控序号相等
          Object rtucolval = RTUTableModel.getValueAt(i, rtucol);
          if (rtucolval != null) {
            if (rtucolval.toString().trim().equalsIgnoreCase(snewyk)) {
              return!checkRet;
            }
          }
        }
      }
      else if(this.getCALCFlagName().equalsIgnoreCase(curFieldName) ){//计算量标志的字段在修改,肯定是由true to falsese
       //没管是否有遥控标志
        if (tmpcmp(i, row, yknocol,YKNoTableModel) == 0) {
          if (tmpcmp(i, row, rtucol,  RTUTableModel) == 0) {
            return!checkRet;
          }
        }
      }
      else if (column == ykflagcol) {//是否遥控的字段在修改
        if (tmpcmp(i, row, yknocol,YKNoTableModel) == 0) {
          if (tmpcmp(i, row, rtucol,  RTUTableModel) == 0) {
            return!checkRet;
          }
        }
      }

    }//for i
    return checkRet;
  }

  public int tmpcmp(int row1, int row2, int column,MyTableModel tablemodel) {
    Object o1 =  tablemodel.getValueAt(row1, column);
    Object o2 = tablemodel.getValueAt(row2, column);

    // If both values are null, return 0.
    if (o1 == null && o2 == null) {
      return 0;
    }
    else if (o1 == null) { // Define null less than everything.
      return -1;
    }
    else if (o2 == null) {
      return 1;
    }
    if (o1.toString().trim().equalsIgnoreCase(o2.toString().trim())) {
      return 0;
    }
    return 1;

  }

}
