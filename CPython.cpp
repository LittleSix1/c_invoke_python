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
	Py_SetPythonHome(L"D:\\anaconda3\\envs\\python3");//指定python.exe位置
	Py_Initialize();
	// 将当前目录加入sys.path  这两行的意思是你的主目录位置：是在工程目录下因此要把.py文件放到该目录下！。也可以自己定义
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append('./')");
	// 导入hello.py模块
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
		//初始化要传入的参数，args配置成传入两个参数的模式
		PyObject* args = PyTuple_New(2);
		//将Long型数据转换成Python可接收的类型
		PyObject* arg1 = PyLong_FromLong(4);
		PyObject* arg2 = PyLong_FromLong(3);
		//将arg1配置为arg带入的第一个参数
		PyTuple_SetItem(args, 0, arg1);
		//将arg1配置为arg带入的第二个参数
		PyTuple_SetItem(args, 1, arg2);
		//传入参数调用函数，并获取返回值
		PyObject* pRet = PyObject_CallObject(pv, args);
		if (pRet)
		{
			//将返回值转换成long型
			long result = PyLong_AsLong(pRet);
			cout << "result:" << result << endl ;
		}
	}

	Py_DECREF(pmodule);

	Py_Finalize();
	return 1;
}

//从C将list传入python
bool InvokeCToPython_list()
{
	Py_SetPythonHome(L"D:\\anaconda3\\envs\\python3");//指定python.exe位置
	Py_Initialize();    // 初始化

	// 将Python工作路径切换到待调用模块所在目录，一定要保证路径名的正确性

	PyRun_SimpleString("import sys");

	PyRun_SimpleString("sys.path.append('./')");


	// 加载模块
	PyObject *pmodule = PyImport_ImportModule("TestC");

	if (!pmodule) // 加载模块失败
	{
		cout << "[ERROR] Python get module failed." << endl;
		return 0;
	}
	cout << "[INFO] Python get module succeed." << endl;

	double CArray[] = {1.2, 4.5, 6.7, 8.9, 1.5, 0.5};

	PyObject *PyList  = PyList_New(6);// 定义与数组等长的PyList对象数组
	PyObject *ArgList = PyTuple_New(1);// 参数个数， 1个
	for(int Index_i = 0; Index_i < PyList_Size(PyList); Index_i++){

		PyList_SetItem(PyList, Index_i, PyFloat_FromDouble(CArray[Index_i]));//给PyList对象的每个元素赋值
	}

	PyObject* pv = PyObject_GetAttrString(pmodule, "PassListFromCToPython");
	if (!pv || !PyCallable_Check(pv))  // 验证是否加载成功
	{
		cout << "[ERROR] Can't find funftion (PassListFromCToPython)" << endl;
		return 0;
	}
	cout << "[INFO] Get function (PassListFromCToPython) succeed." << endl;

	cout<<"C Array Pass Into The Python List:"<<endl;
	PyTuple_SetItem(ArgList, 0, PyList);//将PyList对象放入PyTuple对象中
	PyObject_CallObject(pv, ArgList);//调用函数，完成传递

	Py_DECREF(pmodule);
	Py_Finalize();      //释放资源

	return 0;
}

//将python中的list 返回给C 
bool InvokePythonToC_list()
{
	Py_SetPythonHome(L"D:\\anaconda3\\envs\\python3");//指定python.exe位置
	Py_Initialize();    // 初始化

	// 将Python工作路径切换到待调用模块所在目录，一定要保证路径名的正确性

	PyRun_SimpleString("import sys");

	PyRun_SimpleString("sys.path.append('./')");


	// 加载模块
	PyObject *pmodule = PyImport_ImportModule("TestC");

	if (!pmodule) // 加载模块失败
	{
		cout << "[ERROR] Python get module failed." << endl;
		return 0;
	}
	cout << "[INFO] Python get module succeed." << endl;


	// 加载函数
	PyObject* pv = PyObject_GetAttrString(pmodule, "IntegerListReturn");
	if (!pv || !PyCallable_Check(pv))  // 验证是否加载成功
	{
		cout << "[ERROR] Can't find funftion (IntegerListReturn)" << endl;
		return 0;
	}
	cout << "[INFO] Get function (IntegerListReturn) succeed." << endl;

	// 调用函数
	PyObject* pRet = PyObject_CallObject(pv, NULL);

	if(PyList_Check(pRet)){ //检查是否为List对象

		int SizeOfList = PyList_Size(pRet);//List对象的大小，这里SizeOfList = 3
		for(int Index_i = 0; Index_i < SizeOfList; Index_i++){

			PyObject *ListItem = PyList_GetItem(pRet, Index_i);//获取List对象中的每一个元素

			int NumOfItems = PyList_Size(ListItem);//List对象子元素的大小，这里NumOfItems = 3

			for(int Index_k = 0; Index_k < NumOfItems; Index_k++){

				PyObject *Item = PyList_GetItem(ListItem, Index_k);//遍历List对象中子元素中的每个元素

				cout << PyLong_AsLong(Item) <<" "; //输出元素

				Py_DECREF(Item); //释放空间
			}

			Py_DECREF(ListItem); //释放空间
		}
		cout<<endl;

	}else{

		cout<<"Not a List"<<endl;
	}


	Py_DECREF(pmodule);
	Py_Finalize();      //释放资源

	return 0;
}

//从C中传递Array 到python中
bool InvokeCToPython_Array()
{	
	Py_SetPythonHome(L"D:\\anaconda3\\envs\\python3");//指定python.exe位置
	Py_Initialize();
	import_array();
	// 将Python工作路径切换到待调用模块所在目录，一定要保证路径名的正确性

	PyRun_SimpleString("import sys");

	PyRun_SimpleString("sys.path.append('./')");//python文件位置

	// 加载模块
	PyObject *pmodule = PyImport_ImportModule("TestC");

	if (!pmodule) // 加载模块失败
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

	// 加载函数
	PyObject* pv = PyObject_GetAttrString(pmodule, "PassArrayFromCToPython");
	if (!pv || !PyCallable_Check(pv))  // 验证是否加载成功
	{
		cout << "[ERROR] Can't find funftion (IntegerListReturn)" << endl;
		return 0;
	}
	cout << "[INFO] Get function (IntegerListReturn) succeed." << endl;

	// 调用函数
	PyObject* pRet = PyObject_CallObject(pv, ArgArray);

	Py_DECREF(pmodule);
	Py_DECREF(PyArray);
	Py_DECREF(ArgArray);
	Py_Finalize();
	return 0;
}




//从PYTHON中返还Array 到C中
bool InvokePythonToC_Array()
{	
	Py_SetPythonHome(L"D:\\anaconda3\\envs\\python3");//指定python.exe位置

	Py_Initialize();
	import_array();


	PyRun_SimpleString("import sys");

	PyRun_SimpleString("sys.path.append('./')");//python文件位置


	// 加载模块
	PyObject *pmodule = PyImport_ImportModule("TestC");

	if (!pmodule) // 加载模块失败
	{
		cout << "[ERROR] Python get module failed." << endl;
		return 0;
	}
	cout << "[INFO] Python get module succeed." << endl;


	// 加载函数
	PyObject* pv = PyObject_GetAttrString(pmodule, "IntegerArrayReturn");
	if (!pv || !PyCallable_Check(pv))  // 验证是否加载成功
	{
		cout << "[ERROR] Can't find funftion (IntegerListReturn)" << endl;
		return 0;
	}
	cout << "[INFO] Get function (IntegerArrayReturn) succeed." << endl;

	// 调用函数 得到返回值
	PyObject *pyResult = PyObject_CallObject(pv, NULL);


	if(pyResult){

		//将结果类型转换成数组对象类型
		PyArrayObject* pyResultArr = PyArray_GETCONTIGUOUS((PyArrayObject*)pyResult);

		//从Python中的PyArrayObject解析出数组数据为c的double类型。

		//PyArray_GETCONTIGUOUS
		double *resDataArr = (double *) PyArray_DATA(pyResultArr);

		//int dimNum = PyArray_NDIM(pyResultArr);//返回数组的维度数 一般是2维或者1维 ，3维相当于一个立方体的矩阵  

		npy_intp *pdim = PyArray_DIMS(pyResultArr);//返回数组各维度上的元素个数值 根据测试，pdim[0]代表行， pdim[1]代表列


		//得到元素总数
		//	int nDataCount = 1;
		//	for (int i = 0; i < dimNum; i++)
		//	{
		//		nDataCount *= pdim[i];
		//	}

		//以下是对返回结果的输出显示
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