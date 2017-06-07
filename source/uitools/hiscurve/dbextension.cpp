#include <QDebug>
#include "dbextension.h"
#define C2G(str) QString::fromLocal8Bit(str)
#define G2C(qs)  (const char*)(qs).toLocal8Bit()

DBExtension::DBExtension()
{

}

DBExtension::~DBExtension()
{
	
}

int DBExtension::sqlReadRecorderDir( QString dbName,QString sqlWord,QStringList& content )
{
	FUNC_STR        func;
    CS_DATAFMT      *datafmt=0;
    CS_INT          i,k,ptcnt;
    void            *pt=0;
    CS_CHAR         *charpt;
    CS_TINYINT      *tinypt;
    CS_SMALLINT     *smallpt;
	CS_USHORT		*usmallpt;
    CS_INT          *intpt;
    CS_REAL         *realpt;
    CS_FLOAT        *floatpt;
	CS_RETCODE		retcode;
	//char            fieldValue[256];
	QString         fieldValue;
	
	//qDebug()<<"sqlWord:"<<sqlWord;

	content.clear();
	if(dbName.trimmed().size()==0) return 0;

	strcpy(func.dbname,G2C(dbName));
	func.func = SEL_ISQL_RESULTS;
	sprintf(func.isql,"%s",G2C(sqlWord));
	
	dbApiLocker.lock();
	retcode = SelectIsqlResults(&func,&datafmt,&pt);	
	dbApiLocker.unlock();

    if ((retcode != CS_SUCCEED) || (func.ret_roxnum<1))
	{
		if(datafmt) free(datafmt);
		if(pt) free(pt);
		return 0;
	}

	

	for (i=0;i<func.ret_roxnum;i++)
	{
		ptcnt=i*func.recorderlen;
		for (k=0;k<func.colnum;k++)
		{
			switch ((datafmt+k)->datatype)
			{
			case CS_TINYINT_TYPE:
				{
					tinypt=(CS_TINYINT*)((CS_CHAR*)pt+ptcnt);
					fieldValue=QString("%1").arg(*tinypt);
				}
				break;	
			case CS_SMALLINT_TYPE:
				{
					if ((ptcnt % 2 ) != 0) ptcnt++;
					smallpt=(CS_SMALLINT*)((CS_CHAR*)pt+ptcnt);
					fieldValue=QString("%1").arg(*smallpt);
				}
				break;
			case CS_BINARY_TYPE:
				{
					if ((ptcnt % 2 ) != 0) ptcnt++;
					usmallpt=(CS_USHORT*)((CS_CHAR*)pt+ptcnt);
					fieldValue=QString("%1").arg(*usmallpt);
				}
				break;
			case CS_INT_TYPE:
				{
					if ((ptcnt % 4) != 0)
					{
						ptcnt=ptcnt/4;
						ptcnt=(ptcnt+1)*4;
					}
					intpt=(CS_INT*)((CS_CHAR*)pt+ptcnt);
					fieldValue=QString("%1").arg(*intpt);
				}
				break;
			case CS_REAL_TYPE:
				{
					if ((ptcnt % 4) != 0)
					{
						ptcnt=ptcnt/4;
						ptcnt=(ptcnt+1)*4;
					}
					realpt=(CS_REAL*)((CS_CHAR*)pt+ptcnt);
					fieldValue=QString("%1").arg(*realpt);
				}
				break;
			case CS_FLOAT_TYPE:
				{
					if ((ptcnt % 8) != 0)
					{
						ptcnt=ptcnt/8;
						ptcnt=(ptcnt+1)*8;
					}
					floatpt=(CS_FLOAT*)((CS_CHAR*)pt+ptcnt);
					fieldValue=QString("%1").arg(*floatpt);
				}
				break;
			case CS_CHAR_TYPE:
			case CS_VARCHAR_TYPE://new modi by yh
				{
					charpt=(CS_CHAR*)((CS_CHAR*)pt+ptcnt);
					fieldValue=QObject::tr(charpt);
					fieldValue.trimmed();
				}
				break;
			default:
				{
					qDebug()<<"不能识别的数据类型!!!";
				}
				break;
			} //end switch
			ptcnt+=(datafmt+k)->maxlength;
			content.append(fieldValue);
		}
	} 
	if (pt) free(pt);
	if (datafmt) free(datafmt);
	return func.colnum;
}

int DBExtension::sqlReadRecorderDir( QString dbName,QString sqlWord,QVector<QVariant>& content )
{
	FUNC_STR        func;
    CS_DATAFMT      *datafmt=0;
    CS_INT          i,k,ptcnt;
    void            *pt=0;
    CS_CHAR         *charpt;
    CS_TINYINT      *tinypt;
    CS_SMALLINT     *smallpt;
	CS_USHORT		*usmallpt;
    CS_INT          *intpt;
    CS_REAL         *realpt;
    CS_FLOAT        *floatpt;
	CS_RETCODE		retcode;
	QVariant        fieldValue;
	
	content.clear();
	if(dbName.trimmed().size()==0) return 0;

	strcpy(func.dbname,G2C(dbName));
	func.func = SEL_ISQL_RESULTS;
	sprintf(func.isql,"%s",G2C(sqlWord));
	
	dbApiLocker.lock();
	retcode = SelectIsqlResults(&func,&datafmt,&pt);	
	dbApiLocker.unlock();

    if ((retcode != CS_SUCCEED) || (func.ret_roxnum<1))
	{
		if(datafmt) free(datafmt);
		if(pt) free(pt);
		return 0;
	}

	for (i=0;i<func.ret_roxnum;i++)
	{
		ptcnt=i*func.recorderlen;
		for (k=0;k<func.colnum;k++)
		{
			switch ((datafmt+k)->datatype)
			{
			case CS_TINYINT_TYPE:
				{
					tinypt=(CS_TINYINT*)((CS_CHAR*)pt+ptcnt);
					fieldValue=QVariant(static_cast<uint>(*tinypt));
				}
				break;	
			case CS_SMALLINT_TYPE:
				{
					if ((ptcnt % 2 ) != 0) ptcnt++;
					smallpt=(CS_SMALLINT*)((CS_CHAR*)pt+ptcnt);
					fieldValue=QVariant(*smallpt);
				}
				break;
			case CS_BINARY_TYPE:
				{
					if ((ptcnt % 2 ) != 0) ptcnt++;
					usmallpt=(CS_USHORT*)((CS_CHAR*)pt+ptcnt);
					fieldValue=QVariant(*usmallpt);
				}
				break;
			case CS_INT_TYPE:
				{
					if ((ptcnt % 4) != 0)
					{
						ptcnt=ptcnt/4;
						ptcnt=(ptcnt+1)*4;
					}
					intpt=(CS_INT*)((CS_CHAR*)pt+ptcnt);
					fieldValue=QVariant(*intpt);
				}
				break;
			case CS_REAL_TYPE:
				{
					if ((ptcnt % 4) != 0)
					{
						ptcnt=ptcnt/4;
						ptcnt=(ptcnt+1)*4;
					}
					realpt=(CS_REAL*)((CS_CHAR*)pt+ptcnt);
					fieldValue=QVariant(*realpt);

				}
				break;
			case CS_FLOAT_TYPE:
				{
					if ((ptcnt % 8) != 0)
					{
						ptcnt=ptcnt/8;
						ptcnt=(ptcnt+1)*8;
					}
					floatpt=(CS_FLOAT*)((CS_CHAR*)pt+ptcnt);
					fieldValue=QVariant(*floatpt);
				}
				break;
			case CS_CHAR_TYPE:
			case CS_VARCHAR_TYPE://new modi by yh
				{
					charpt=(CS_CHAR*)((CS_CHAR*)pt+ptcnt);
					QString strTmp(C2G(charpt));
					strTmp.trimmed();
					fieldValue=(strTmp);
				}
				break;
			default:
				{
					qDebug()<<"不能识别的数据类型!!!";
				}
				break;
			} //end switch
			ptcnt+=(datafmt+k)->maxlength;
			content.append(fieldValue);
		}
	} 
	if (pt) free(pt);
	if (datafmt) free(datafmt);
	return func.colnum;
}



bool DBExtension::InsertRecords( QString dbName,QString tableName,QStringList& fieldName,QStringList& fieldValue,int recNums /*= 0*/ )
{
	FUNC_STR func;
	CS_DATAFMT *datafmt;
	func.func = GET_TABLE_INFO;
	strcpy(func.table,G2C(tableName));
	strcpy(func.dbname,G2C(dbName));
	
	if(dbName.isEmpty() || dbName.isNull() || dbName.trimmed().size()==0) 
	{
		dbApiLocker.unlock();
		return false;
	}

	if(tableName.isEmpty() || tableName.isNull() || tableName.trimmed().size()==0) 
	{
		dbApiLocker.unlock();
		return false;
	}
	
	dbApiLocker.lock();
	
	if(GetTableInfo(&func,&datafmt) == CS_FAIL)
	{
		free(datafmt);
		dbApiLocker.unlock();
		return false;
	}
	
	bool bAllFieldFlag = false;
	int fieldNums = 0;	//域的数量
	if(fieldName[0] == C2G("*"))
	{
		bAllFieldFlag = true;
		fieldNums = func.colnum;
	}
	else
		fieldNums = fieldName.size();
	
	if(fieldNums == 0) 
	{
		dbApiLocker.unlock();
		return false;
	}
	
	if(recNums != 0)
	{
		if(fieldValue.size() / fieldNums != recNums)
		{
			dbApiLocker.unlock();
			return false; 
		}
	}
	else
		recNums = fieldValue.size() / fieldNums;
	
	
	int ptcnt = 0;
	char*  pt = (char*)malloc(func.recorderlen*recNums);
	Q_ASSERT(pt != NULL);
	memset(pt,0,func.recorderlen*recNums);
	
	int *pColToIndex = NULL;
	if(!bAllFieldFlag)//不是全部的域
	{
		int realcount = 0;
		pColToIndex = new int[func.colnum]; //数据库字段对应到 fieldname中的字段序号
		Q_ASSERT(pColToIndex != NULL);
		
		for(int k = 0; k < func.colnum; k++)
		{
			pColToIndex[k] = -1;
			for(int i = 0; i < fieldNums; i++)
			{
				//qDebug()<<fieldName[i] << "  " << C2G(datafmt[k].name);
				if(fieldName[i] == C2G(datafmt[k].name))
				{
					pColToIndex[k] = i;
					realcount ++;
					break;
				}
			}
		}
		
		if(realcount != fieldNums) 
		{
			dbApiLocker.unlock();
			return false;	//数据库字段中没有找到fieldname中的某些域
		}
		
	}
	QString tmpval;
	
	for(int row = 0; row < recNums; row++)
	{
		for(int k = 0; k < func.colnum; k++)
		{
			if(!bAllFieldFlag)//不是全部的域
			{
				if(pColToIndex[k] == -1) 
				{
					tmpval = "";	//没有该字段,不能跳过
				}
				else
					tmpval = fieldValue[fieldNums * row + pColToIndex[k]];
			}
			else
				tmpval = fieldValue[fieldNums * row + k];
			
			switch ((datafmt+k)->datatype)
			{
			case CS_TINYINT_TYPE:
				{
					char tinyintval = (char)atoi(G2C(tmpval));
					FillData(datafmt+k,pt,ptcnt,&tinyintval);
				}
				break;
			case CS_SMALLINT_TYPE:
				{
					short shortintval = (short)atoi(G2C(tmpval));
					FillData(datafmt+k,pt,ptcnt,&shortintval);
				}
				break;
			case CS_INT_TYPE:
				{
					int intval = (int)atoi(G2C(tmpval));
					FillData(datafmt+k,pt,ptcnt,&intval);
				}
				break;
			case CS_LONG_TYPE:
				{
					long longintval = (long)atoi(G2C(tmpval));
					FillData(datafmt+k,pt,ptcnt,&longintval);
				}
				break;
			case CS_REAL_TYPE:
			/*{
			break;
			}*/
			case CS_FLOAT_TYPE:
				{
					float floatval = (float)atof(G2C(tmpval));
					FillData(datafmt+k,pt,ptcnt,&floatval);
				}
				break;
			case CS_BINARY_TYPE:
			case CS_CHAR_TYPE:
			case CS_VARCHAR_TYPE:
				{
					FillData(datafmt+k, pt, ptcnt, (char*)G2C(tmpval));
				}
				break;
			case CS_DECIMAL_TYPE:
				{
				}
				break;
			}//switch
		}//for k
	}
	func.func = BLK_IN;
	func.roxnum = recNums;
	
	free(pColToIndex);
	if(BlkCopyIn(&func,datafmt,pt,1) != CS_SUCCEED)
	{
		free(pt);
		free(datafmt);
		dbApiLocker.unlock();
		return false;
	}
	free(pt);
	free(datafmt);
	dbApiLocker.unlock();
	return true;
}

bool DBExtension::Execute( QString dbName,QString sqlStatement )
{
	FUNC_STR    func;
	CS_INT		retcode;
	
	if(dbName.isEmpty() || dbName.isNull() || dbName.trimmed().size()==0) return false;
	if(sqlStatement.isEmpty() || sqlStatement.isNull() || sqlStatement.trimmed().size()==0) return false;
	
	strcpy(func.dbname,G2C(dbName));
    func.func=ISQL_COMMAND;
	
	strcpy(func.isql,G2C(sqlStatement));
	dbApiLocker.lock();
	retcode=IsqlCommand(&func);
	dbApiLocker.unlock();
   	return retcode;
}

