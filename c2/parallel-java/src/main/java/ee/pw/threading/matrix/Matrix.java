package ee.pw.threading.matrix;

import java.io.InputStream;
import java.util.Scanner;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;

public class Matrix {
    private final int numberOfColumns;
    private final int numberOfRows;
    private final double[][] matrixData;
    private double frobeniusNorm;

    public Matrix(int numberOfRows, int numberOfColumns) {
        this.numberOfColumns = numberOfColumns;
        this.numberOfRows = numberOfRows;
        this.matrixData = new double[numberOfRows][numberOfColumns];
        this.frobeniusNorm = 0.0;
    }

    public Matrix(int numberOfColumns, int numberOfRows, double[][] matrixData) {
        this.numberOfColumns = numberOfColumns;
        this.numberOfRows = numberOfRows;
        this.matrixData = matrixData;
        this.frobeniusNorm = 0.0;
    }

    public int getNumberOfColumns() {
        return numberOfColumns;
    }

    public int getNumberOfRows() {
        return numberOfRows;
    }

    public void setMatrixElement(int rowNum, int colNum, double value) {
        matrixData[rowNum][colNum] = value;
    }

    public double getMatrixElement(int row, int column) {
        return matrixData[row][column];
    }

    public void printMatrix() {
        if (this.matrixData == null) {
            throw new IllegalStateException("This argument cannot be null to print the matrix!");
        }

        for (int i = 0; i < numberOfRows; ++i) {
            for (int j = 0; j < numberOfColumns; ++j) {
                System.out.print(String.format("%g ", getMatrixElement(i, j)));
            }

            System.out.println();
        }
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
        final ExecutorService multiplyThreadPool = Executors.newFixedThreadPool(numberOfThreads);
        final int singleThreadRowStep = firstMatrix.getNumberOfRows() / numberOfThreads;

        for (int i = 0; i < numberOfThreads; ++i) {
            final int currentIndex = i;

            final Runnable singleThreadMultiplyTask = () -> {
                int startingRow = currentIndex * singleThreadRowStep;
                int endingRow = currentIndex == numberOfThreads - 1 ? firstMatrix.getNumberOfRows() : (currentIndex + 1) * singleThreadRowStep;

                for (int row = startingRow; row < endingRow; ++row) {
                    for (int column = 0; column < secondMatrix.getNumberOfColumns(); ++column) {
                        double elementValue = 0;

                        for (int k = 0; k < firstMatrix.getNumberOfColumns(); ++k) {
                            elementValue += firstMatrix.getMatrixElement(row, k) * secondMatrix.getMatrixElement(k, column);
                        }

                        synchronized (outputMatrix) {
                            outputMatrix.setMatrixElement(row, column, elementValue);
                            outputMatrix.addElementToFrobenius(elementValue);
                        }
                    }
                }
            };

            multiplyThreadPool.execute(singleThreadMultiplyTask);
        }

        multiplyThreadPool.shutdown();

        try {
            multiplyThreadPool.awaitTermination(1000, TimeUnit.MILLISECONDS);
        } catch (InterruptedException ex) {
        }


        return outputMatrix;
    }

    private synchronized void addElementToFrobenius(double element) {
        frobeniusNorm += Math.pow(element, 2);
    }

    public static void validateMatrixMultiplication(Matrix firstMatrix, Matrix secondMatrix) {
        if (firstMatrix.getNumberOfColumns() != secondMatrix.getNumberOfRows()) {
            throw new IllegalStateException("There should not be situation in which first number rows are different from second matrix columns while multiplying!");
        }
    }

    public double getFrobeniusNorm() {
        return Math.sqrt(frobeniusNorm);
    }
}