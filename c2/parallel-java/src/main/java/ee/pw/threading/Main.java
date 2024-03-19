package ee.pw.threading;

import java.io.InputStream;
import java.util.Scanner;

import ee.pw.threading.matrix.Matrix;

public class Main {

    public static void main(String... args) {
        final Scanner scanner = new Scanner(System.in);
        System.out.println("Please provide the number of threads in the program:");
        final int numberOfThreads = scanner.nextInt();
        System.out.println("Please provide the first matrix file: ");
        final InputStream resourceFirstMatrixFileName = ClassLoader.getSystemResourceAsStream(scanner.next());
        System.out.println("Please provide the second matrix file name in resources:");
        final InputStream resourceSecondMatrixFileName = ClassLoader.getSystemResourceAsStream(scanner.next());

        final Matrix firstMatrix = Matrix.ofFileName(resourceFirstMatrixFileName);
        final Matrix secondMatrix = Matrix.ofFileName(resourceSecondMatrixFileName);

        System.out.println("First loaded matrix to multiply: ");
        firstMatrix.printMatrix();
        System.out.println();

        System.out.println("Second loaded matrix to multiply: ");
        secondMatrix.printMatrix();
        System.out.println();

        final Matrix resultMatrix = Matrix.multiplyMatrixesParallel(firstMatrix, secondMatrix, numberOfThreads);

        System.out.println("Result matrix after multiplying: ");
        resultMatrix.printMatrix();
        System.out.println();
        System.out.println(String.format("Frobenius value of the given matrix is: %g", resultMatrix.getFrobeniusNorm()));
    }

}
