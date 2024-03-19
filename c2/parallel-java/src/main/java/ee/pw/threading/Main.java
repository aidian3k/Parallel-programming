package ee.pw.threading;

import ee.pw.threading.matrix.Matrix;

import java.io.InputStream;

public class Main {

    public static void main(String... args) {
        validateInputArguments(args);

        final InputStream resourceFirstMatrixFileName = ClassLoader.getSystemResourceAsStream(args[0]);
        final InputStream resourceSecondMatrixFileName = ClassLoader.getSystemResourceAsStream(args[1]);
        final int numberOfThreads = Integer.parseInt(args[2]);

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

        System.out.printf("Frobenius value of the given matrix is: %g%n", resultMatrix.getFrobeniusNorm());
    }

    private static void validateInputArguments(String... args) {
        if (args.length < 3) {
            throw new IllegalStateException("There should be three arguments to start the program: " +
                    "[file_name_of_matrix_a], [file_name_of_matrix_b], [number_of_threads]");
        }
    }

}
