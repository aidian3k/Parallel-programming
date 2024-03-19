package ee.pw.threading.matrix;

import java.io.InputStream;
import java.util.Scanner;

public final class Matrix {
    private final int numberOfColumns;
    private final int numberOfRows;
    private final double[][] matrixData;
    private double frobeniusNormSum;

    public Matrix(int numberOfRows, int numberOfColumns) {
        this.numberOfColumns = numberOfColumns;
        this.numberOfRows = numberOfRows;
        this.matrixData = new double[numberOfRows][numberOfColumns];
        this.frobeniusNormSum = 0;
    }

    public Matrix(int numberOfColumns, int numberOfRows, double[][] matrixData) {
        this.numberOfColumns = numberOfColumns;
        this.numberOfRows = numberOfRows;
        this.matrixData = matrixData;
        this.frobeniusNormSum = 0;
    }

    public static Matrix ofFileName(InputStream matrixInputStream) {
        try (final Scanner matrixScanner = new Scanner(matrixInputStream)) {
            final int numberOfRows = matrixScanner.nextInt();
            final int numberOfColumns = matrixScanner.nextInt();
            double[][] matrixData = new double[numberOfRows][numberOfColumns];

            for (int i = 0; i < numberOfRows; ++i) {
                for (int j = 0; j < numberOfColumns; ++j) {
                    matrixData[i][j] = matrixScanner.nextDouble();
                }
            }

            return new Matrix(numberOfColumns, numberOfRows, matrixData);
        }
    }

    public static Matrix multiplyMatrixesParallel(Matrix firstMatrix, Matrix secondMatrix, int numberOfThreads) {
        validateMatrixMultiplication(firstMatrix, secondMatrix);

        final Matrix outputMatrix = new Matrix(firstMatrix.getNumberOfRows(), secondMatrix.getNumberOfColumns());
        final Thread[] threadPool = new Thread[numberOfThreads];
        double threadFrobeniusNormTask = 0;
        final int cellsOfOutputMatrix = outputMatrix.getNumberOfColumns() * outputMatrix.getNumberOfRows();
        final int cellsPerThread = cellsOfOutputMatrix / numberOfThreads;

        for (int i = 0; i < numberOfThreads; ++i) {
            final int currentIndex = i;

            final Runnable singleThreadMultiplyTask = () -> {
                double frobeniusThreadTaskSum = 0;
                final int startingThreadWorkCell = currentIndex * cellsPerThread;
                final int endingThreadWorkCell =
                        currentIndex == numberOfThreads - 1 ? cellsOfOutputMatrix : (currentIndex + 1) * cellsPerThread;

                for (int cell = startingThreadWorkCell; cell < endingThreadWorkCell; ++cell) {
                    int row = cell / outputMatrix.getNumberOfColumns();
                    int column = cell % outputMatrix.getNumberOfColumns();
                    double elementValue = 0;

                    for (int k = 0; k < firstMatrix.getNumberOfColumns(); ++k) {
                        elementValue += firstMatrix.getMatrixElement(row, k) * secondMatrix.getMatrixElement(k, column);
                    }

                    outputMatrix.setMatrixElement(row, column, elementValue);
                    frobeniusThreadTaskSum += Math.pow(elementValue, 2);
                }

                outputMatrix.addCalculatedFrobeniusTaskSumToSum(frobeniusThreadTaskSum);
            };

            threadPool[i] = new Thread(singleThreadMultiplyTask);
            threadPool[i].start();
        }

        for (int i = 0; i < numberOfThreads; ++i) {
            try {
                threadPool[i].join();
            } catch (InterruptedException interruptedException) {
                System.err.println("There was an interruption error during joining threadPool!");
            }
        }

        return outputMatrix;
    }

    private static void validateMatrixMultiplication(Matrix firstMatrix, Matrix secondMatrix) {
        if (firstMatrix.getNumberOfColumns() != secondMatrix.getNumberOfRows()) {
            throw new IllegalStateException("There should not be situation in which first number rows are different " +
                    "from second matrix columns while multiplying!");
        }
    }

    public int getNumberOfColumns() {
        return numberOfColumns;
    }

    public int getNumberOfRows() {
        return numberOfRows;
    }

    public double getMatrixElement(int row, int column) {
        return matrixData[row][column];
    }

    public void setMatrixElement(int rowNum, int colNum, double value) {
        matrixData[rowNum][colNum] = value;
    }

    public double getFrobeniusNorm() {
        return Math.sqrt(frobeniusNormSum);
    }

    public void printMatrix() {
        if (this.matrixData == null) {
            throw new IllegalStateException("This argument cannot be null to print the matrix!");
        }

        for (int i = 0; i < numberOfRows; ++i) {
            for (int j = 0; j < numberOfColumns; ++j) {
                System.out.printf("%g ", getMatrixElement(i, j));
            }

            System.out.println();
        }
    }

    private synchronized void addCalculatedFrobeniusTaskSumToSum(double singleFrobeniusTaskOutputSum) {
        this.frobeniusNormSum += singleFrobeniusTaskOutputSum;
    }
}