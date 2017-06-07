package com.daqo.dbinfo.table;

import com.daqo.dbinfo.*;
import javax.swing.JOptionPane;

/**
 * Description: 数据库维护工具
 * Company: daqo
 * version 1.0
 */

public class PDB_Table {

  public PDB_Table() {
    FieldProp pdbf_table = new FieldProp(ConstantDef.MODELDB,
                                         ConstantDef.PARA_TABLE_DESCRIBLE);
    pdbf_table.getTableInfo(); //得到pdb_field表信息

    ConstantDef.TABLENAMEID = pdbf_table.getColumnNo(ConstantDef.TABLENAME); //TABLENAME在pdbf_table中的列号
    ConstantDef.APPTYPEID = pdbf_table.getColumnNo(ConstantDef.APPTYPE); //APPTYPE在pdbf_table中的列号
    ConstantDef.LISTFIELDID = pdbf_table.getColumnNo(ConstantDef.LISTFIELD); //LISTFIELD在pdbf_table中的列号
    ConstantDef.SORTFIELDID = pdbf_table.getColumnNo(ConstantDef.SORTFIELD); //SORTFIELD在pdbf_table中的列号
    ConstantDef.SORTINDEXID = pdbf_table.getColumnNo(ConstantDef.SORTINDEX); //SORTINDEX在pdbf_table中的列号

    ConstantDef.SEARCHFIELDID = pdbf_table.getColumnNo(ConstantDef.SEARCHFIELD); //SEARCHFIELD在pdbf_table中的列号
    ConstantDef.SEARCHEDTABLEID = pdbf_table.getColumnNo(ConstantDef.
        SEARCHEDTABLE); //SEARCHEDTABLE在pdbf_table中的列号
    ConstantDef.SEARCHEDFIELDID = pdbf_table.getColumnNo(ConstantDef.
        SEARCHEDFIELD); //SEARCHEDFIELD在pdbf_table中的列号
    ConstantDef.SHOWFIELDID = pdbf_table.getColumnNo(ConstantDef.SHOWFIELD); //SHOWFIELD在pdbf_table中的列号
    ConstantDef.TABCOUNTID = pdbf_table.getColumnNo(ConstantDef.TABCOUNT); //TABCOUNT在pdbf_table中的列号

    ConstantDef.TAB1ID = pdbf_table.getColumnNo(ConstantDef.TAB1); //TAB1在pdbf_table中的列号
    ConstantDef.TAB2ID = pdbf_table.getColumnNo(ConstantDef.TAB2); //TAB2在pdbf_table中的列号
    ConstantDef.TAB3ID = pdbf_table.getColumnNo(ConstantDef.TAB3); //TAB3在pdbf_table中的列号
    ConstantDef.TAB4ID = pdbf_table.getColumnNo(ConstantDef.TAB4); //TAB4在pdbf_table中的列号
    ConstantDef.TAB5ID = pdbf_table.getColumnNo(ConstantDef.TAB5); //TAB5在pdbf_table中的列号
  }
}
