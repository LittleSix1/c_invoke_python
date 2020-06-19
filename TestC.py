import numpy as np
def Hello():
    print ('Hello')
	
def test_add(a, b):
    print ('a+b')
    return a+b
	

def IntegerListReturn():
    IntegerList = [[1, 2, 3], [4, 5, 6], [7, 8, 9]]
    return IntegerList
	
	
def IntegerArrayReturn():
    IntegerList = [[1, 2, 3],[4, 5, 6], [7, 8, 9]]
    Array_A  = np.asarray(IntegerList, dtype='float' )
    #Array_A = np.squeeze(np.reshape(Array_A, (1, -1)))
    return Array_A

	
def PassListFromCToPython(List):
    PyList = List
    print (PyList)

def PassArrayFromCToPython(Array):
    print ('Shape Of Array:',Array.shape)
    print (Array)