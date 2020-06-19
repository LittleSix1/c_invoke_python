#include "stdafx.h"
#include <Python.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
using namespace std;
#include <numpy/arrayobject.h>
#if 1

bool InvokePython()
{
	Py_SetPythonHome(L"D:\\anaconda3\\envs\\python3");//ָ��python.exeλ��
	Py_Initialize();
	// ����ǰĿ¼����sys.path  �����е���˼�������Ŀ¼λ�ã����ڹ���Ŀ¼�����Ҫ��.py�ļ��ŵ���Ŀ¼�£���Ҳ�����Լ�����
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append('./')");
	// ����hello.pyģ��
	PyObject *pmodule = PyImport_ImportModule("TestC");
	if (!pmodule)
	{
		printf("can't find pytest.py");
		return 1;
	}
	{
		PyObject* pFunc = PyObject_GetAttrString(pmodule, "Hello");
		PyEval_CallObject(pFunc, NULL);
		Py_DECREF(pFunc);
	}
	{
		PyObject* pv = PyObject_GetAttrString(pmodule, "test_add");
		if (!pv || !PyCallable_Check(pv))
		{
			cout << "Can't find funftion (test_add)" << endl;
			return 0;
		}
		cout << "Get function (test_add) succeed." << endl;
		//��ʼ��Ҫ����Ĳ�����args���óɴ�������������ģʽ
		PyObject* args = PyTuple_New(2);
		//��Long������ת����Python�ɽ��յ�����
		PyObject* arg1 = PyLong_FromLong(4);
		PyObject* arg2 = PyLong_FromLong(3);
		//��arg1����Ϊarg����ĵ�һ������
		PyTuple_SetItem(args, 0, arg1);
		//��arg1����Ϊarg����ĵڶ�������
		PyTuple_SetItem(args, 1, arg2);
		//����������ú���������ȡ����ֵ
		PyObject* pRet = PyObject_CallObject(pv, args);
		if (pRet)
		{
			//������ֵת����long��
			long result = PyLong_AsLong(pRet);
			cout << "result:" << result << endl ;
		}
	}

	Py_DECREF(pmodule);

	Py_Finalize();
	return 1;
}

//��C��list����python
bool InvokeCToPython_list()
{
	Py_SetPythonHome(L"D:\\anaconda3\\envs\\python3");//ָ��python.exeλ��
	Py_Initialize();    // ��ʼ��

	// ��Python����·���л���������ģ������Ŀ¼��һ��Ҫ��֤·��������ȷ��

	PyRun_SimpleString("import sys");

	PyRun_SimpleString("sys.path.append('./')");


	// ����ģ��
	PyObject *pmodule = PyImport_ImportModule("TestC");

	if (!pmodule) // ����ģ��ʧ��
	{
		cout << "[ERROR] Python get module failed." << endl;
		return 0;
	}
	cout << "[INFO] Python get module succeed." << endl;

	double CArray[] = {1.2, 4.5, 6.7, 8.9, 1.5, 0.5};

	PyObject *PyList  = PyList_New(6);// ����������ȳ���PyList��������
	PyObject *ArgList = PyTuple_New(1);// ���������� 1��
	for(int Index_i = 0; Index_i < PyList_Size(PyList); Index_i++){

		PyList_SetItem(PyList, Index_i, PyFloat_FromDouble(CArray[Index_i]));//��PyList�����ÿ��Ԫ�ظ�ֵ
	}

	PyObject* pv = PyObject_GetAttrString(pmodule, "PassListFromCToPython");
	if (!pv || !PyCallable_Check(pv))  // ��֤�Ƿ���سɹ�
	{
		cout << "[ERROR] Can't find funftion (PassListFromCToPython)" << endl;
		return 0;
	}
	cout << "[INFO] Get function (PassListFromCToPython) succeed." << endl;

	cout<<"C Array Pass Into The Python List:"<<endl;
	PyTuple_SetItem(ArgList, 0, PyList);//��PyList�������PyTuple������
	PyObject_CallObject(pv, ArgList);//���ú�������ɴ���

	Py_DECREF(pmodule);
	Py_Finalize();      //�ͷ���Դ

	return 0;
}

//��python�е�list ���ظ�C 
bool InvokePythonToC_list()
{
	Py_SetPythonHome(L"D:\\anaconda3\\envs\\python3");//ָ��python.exeλ��
	Py_Initialize();    // ��ʼ��

	// ��Python����·���л���������ģ������Ŀ¼��һ��Ҫ��֤·��������ȷ��

	PyRun_SimpleString("import sys");

	PyRun_SimpleString("sys.path.append('./')");


	// ����ģ��
	PyObject *pmodule = PyImport_ImportModule("TestC");

	if (!pmodule) // ����ģ��ʧ��
	{
		cout << "[ERROR] Python get module failed." << endl;
		return 0;
	}
	cout << "[INFO] Python get module succeed." << endl;


	// ���غ���
	PyObject* pv = PyObject_GetAttrString(pmodule, "IntegerListReturn");
	if (!pv || !PyCallable_Check(pv))  // ��֤�Ƿ���سɹ�
	{
		cout << "[ERROR] Can't find funftion (IntegerListReturn)" << endl;
		return 0;
	}
	cout << "[INFO] Get function (IntegerListReturn) succeed." << endl;

	// ���ú���
	PyObject* pRet = PyObject_CallObject(pv, NULL);

	if(PyList_Check(pRet)){ //����Ƿ�ΪList����

		int SizeOfList = PyList_Size(pRet);//List����Ĵ�С������SizeOfList = 3
		for(int Index_i = 0; Index_i < SizeOfList; Index_i++){

			PyObject *ListItem = PyList_GetItem(pRet, Index_i);//��ȡList�����е�ÿһ��Ԫ��

			int NumOfItems = PyList_Size(ListItem);//List������Ԫ�صĴ�С������NumOfItems = 3

			for(int Index_k = 0; Index_k < NumOfItems; Index_k++){

				PyObject *Item = PyList_GetItem(ListItem, Index_k);//����List��������Ԫ���е�ÿ��Ԫ��

				cout << PyLong_AsLong(Item) <<" "; //���Ԫ��

				Py_DECREF(Item); //�ͷſռ�
			}

			Py_DECREF(ListItem); //�ͷſռ�
		}
		cout<<endl;

	}else{

		cout<<"Not a List"<<endl;
	}


	Py_DECREF(pmodule);
	Py_Finalize();      //�ͷ���Դ

	return 0;
}

//��C�д���Array ��python��
bool InvokeCToPython_Array()
{	
	Py_SetPythonHome(L"D:\\anaconda3\\envs\\python3");//ָ��python.exeλ��
	Py_Initialize();
	import_array();
	// ��Python����·���л���������ģ������Ŀ¼��һ��Ҫ��֤·��������ȷ��

	PyRun_SimpleString("import sys");

	PyRun_SimpleString("sys.path.append('./')");//python�ļ�λ��

	// ����ģ��
	PyObject *pmodule = PyImport_ImportModule("TestC");

	if (!pmodule) // ����ģ��ʧ��
	{
		cout << "[ERROR] Python get module failed." << endl;
		return 0;
	}
	cout << "[INFO] Python get module succeed." << endl;


	double CArrays[3][3] = {{1.3, 2.4, 5.6}, {4.5, 7.8, 8.9}, {1.7, 0.4, 0.8}};
	npy_intp Dims[2] = {3, 3};

	PyObject *PyArray  = PyArray_SimpleNewFromData(2, Dims, NPY_DOUBLE, CArrays);
	PyObject *ArgArray = PyTuple_New(1);
	PyTuple_SetItem(ArgArray, 0, PyArray);

	// ���غ���
	PyObject* pv = PyObject_GetAttrString(pmodule, "PassArrayFromCToPython");
	if (!pv || !PyCallable_Check(pv))  // ��֤�Ƿ���سɹ�
	{
		cout << "[ERROR] Can't find funftion (IntegerListReturn)" << endl;
		return 0;
	}
	cout << "[INFO] Get function (IntegerListReturn) succeed." << endl;

	// ���ú���
	PyObject* pRet = PyObject_CallObject(pv, ArgArray);

	Py_DECREF(pmodule);
	Py_DECREF(PyArray);
	Py_DECREF(ArgArray);
	Py_Finalize();
	return 0;
}




//��PYTHON�з���Array ��C��
bool InvokePythonToC_Array()
{	
	Py_SetPythonHome(L"D:\\anaconda3\\envs\\python3");//ָ��python.exeλ��

	Py_Initialize();
	import_array();


	PyRun_SimpleString("import sys");

	PyRun_SimpleString("sys.path.append('./')");//python�ļ�λ��


	// ����ģ��
	PyObject *pmodule = PyImport_ImportModule("TestC");

	if (!pmodule) // ����ģ��ʧ��
	{
		cout << "[ERROR] Python get module failed." << endl;
		return 0;
	}
	cout << "[INFO] Python get module succeed." << endl;


	// ���غ���
	PyObject* pv = PyObject_GetAttrString(pmodule, "IntegerArrayReturn");
	if (!pv || !PyCallable_Check(pv))  // ��֤�Ƿ���سɹ�
	{
		cout << "[ERROR] Can't find funftion (IntegerListReturn)" << endl;
		return 0;
	}
	cout << "[INFO] Get function (IntegerArrayReturn) succeed." << endl;

	// ���ú��� �õ�����ֵ
	PyObject *pyResult = PyObject_CallObject(pv, NULL);


	if(pyResult){

		//���������ת���������������
		PyArrayObject* pyResultArr = PyArray_GETCONTIGUOUS((PyArrayObject*)pyResult);

		//��Python�е�PyArrayObject��������������Ϊc��double���͡�

		//PyArray_GETCONTIGUOUS
		double *resDataArr = (double *) PyArray_DATA(pyResultArr);

		//int dimNum = PyArray_NDIM(pyResultArr);//���������ά���� һ����2ά����1ά ��3ά�൱��һ��������ľ���  

		npy_intp *pdim = PyArray_DIMS(pyResultArr);//���������ά���ϵ�Ԫ�ظ���ֵ ���ݲ��ԣ�pdim[0]�����У� pdim[1]������


		//�õ�Ԫ������
		//	int nDataCount = 1;
		//	for (int i = 0; i < dimNum; i++)
		//	{
		//		nDataCount *= pdim[i];
		//	}

		//�����ǶԷ��ؽ���������ʾ
		for (int i = 0; i < pdim[1]; ++i) {

			for (int j = 0; j < pdim[0]; ++j)
				cout << resDataArr[i * pdim[0] + j] << ",";
		}
		cout << endl;

	}else{

		cout<<"Not a List"<<endl;
	}


	Py_DECREF(pmodule);
	Py_DECREF(pyResult);
	Py_Finalize();
	return 0;
}



int main()
{

	//InvokePython();
	//InvokeCToPython_list();
	//InvokePythonToC_list();
	//InvokeCToPython_Array();
	InvokePythonToC_Array();
	getchar();
	return 0;
}
#endif