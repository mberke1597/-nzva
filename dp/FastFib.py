import numpy as np

def fibonacci_matrix(n):
	def matrix_power(matrix, power):
		return np.linalg.matrix_power(matrix, power)
	if n == 0:
		return 0
	matrix = np.array([[1, 1], [1, 0]])
	result = matrix_power(matrix, n-1)
	return result[0][0]

fibonacci_numbers = []
n = int(input())
print(fibonacci_matrix(n))