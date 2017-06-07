package com.daqo.dbinfo;

/**
 * Description: 数据库维护工具
 * Company: daqo
 * version 1.0
 */

public class User {

  private int operatorGroup; //操作员类别
  private String groupDesc;//类别描述
  private String operator;//  操作员
  private String desc;//描述
  private String groupName;//组名
  private int mainManFlag; //组长标志

  private int editParaFlag; //修改数据库参数
  private int browserParaFlag; //查看数据库参数

  private int superdoorFlag; //超级用户

  private boolean blongf = false; //是否已登录

  public User() {
    initUser();
  }


  public void initUser() {
    operatorGroup = -1;
    groupDesc = "";
    operator = "";
    desc = "";
    groupName= "";
    mainManFlag = 0;

    editParaFlag = 0;
    browserParaFlag = 0;

    superdoorFlag = 0;
  }


//组名
public void setGroupName(String groupName)
    {
      this.groupName = groupName;
    }
public String getGroupName()
    {
      return this.groupName ;
    }


//描述
public void setDesc(String desc)
    {
      this.desc = desc;
    }
public String getDesc()
    {
      return this.desc ;
    }

//操作员
public void  setOperator(String operator)
    {
      this.operator = operator;
    }
public String getOperator()
    {
      return this.operator ;
    }


//类别描述
public void setGroupDesc(String groupDesc)
    {
      this.groupDesc = groupDesc;
    }
public String getGroupDesc()
    {
      return this.groupDesc ;
    }

  //操作员类别
  public void setGroup(int group) {
    operatorGroup = group;
  }

  public int getGroup() {
    return operatorGroup;
  }


  public void setEditPara(int editParaRight) {
    editParaFlag = editParaRight;
  }

  public int getEditPara() {
    return editParaFlag;
  }

  public void setBrowserPara(int browserParaRight) {
    browserParaFlag = browserParaRight;
  }

  public int getBrowserPara() {
    return browserParaFlag;
  }


  //是否管理员组
  public boolean isManageGroup() {
    return operatorGroup == 0;
  }

  public void setMainman(int mainFlag) {
    mainManFlag = mainFlag;
  }

  public int getMainman() {
    return mainManFlag;
  }

  public void setSuperdoor(int superFlag) {
    superdoorFlag = superFlag;
    if (superdoorFlag > 0) { //superdoor只是管理员组的一员
      editParaFlag = 1;
      browserParaFlag = 1;
      operatorGroup = 0;
      mainManFlag = 1;
    }
  }

  public int getSuperdoor() {
    return superdoorFlag;
  }

  public int getEditRight() {
    return superdoorFlag + editParaFlag;
  }

  public void setLoginF(boolean longinf)
      {
        this.blongf =longinf;
      }

  public boolean getLoginF()
      {
        return this.blongf ;
      }

}
