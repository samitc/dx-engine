#include "gtest\gtest.h"
#include "DXGame.h"
#include "Vector.h"
#include "Matrix.h"
#include "GeneralException.h"
namespace GeneralTesting
{
    static void checkVector(Vector &v1, Vector &v2);
    TEST(VectorTesting, getVector)
    {
        Vector v = Vector(2, 3, 4, 5);
        ASSERT_EQ(2.0f, v.getX());
        ASSERT_EQ(3.0f, v.getY());
        ASSERT_EQ(4.0f, v.getZ());
        ASSERT_EQ(5.0f, v.getW());
    }
    TEST(VectorTesting, regCtor)
    {
        Vector v0 = Vector();
        ASSERT_EQ(0.0f, v0.getX());
        ASSERT_EQ(0.0f, v0.getY());
        ASSERT_EQ(0.0f, v0.getZ());
        ASSERT_EQ(0.0f, v0.getW());
    }
    TEST(VectorTesting, copy)
    {
        Vector v1 = Vector(5, 87.3, 2.4, 7);
        Vector v = v1;
        ASSERT_EQ(5.0f, v.getX());
        ASSERT_EQ(87.3f, v.getY());
        ASSERT_EQ(2.4f, v.getZ());
        ASSERT_EQ(7.0f, v.getW());
    }
    TEST(VectorTesting, copyCtor)
    {
        Vector v = Vector(5, 87.3, 2.4, 7);
        Vector v2 = Vector(v);
        ASSERT_EQ(5.0f, v2.getX());
        ASSERT_EQ(87.3f, v2.getY());
        ASSERT_EQ(2.4f, v2.getZ());
        ASSERT_EQ(7.0f, v2.getW());
    }
    TEST(VectorTesting, addingVector)
    {
        Vector v2 = Vector(5, 87.3, 2.4, 7);
        Vector v3 = Vector(1.5f, 3.5f, 1.6f, 3.4f);
        Vector v4 = v2 + v3;
        ASSERT_EQ(6.5f, v4.getX());
        ASSERT_EQ(90.8f, v4.getY());
        ASSERT_EQ(4.0f, v4.getZ());
        ASSERT_EQ(10.4f, v4.getW());
        v2 = v2 + v3;
        ASSERT_EQ(v4.getX(), v2.getX());
        ASSERT_EQ(v4.getY(), v2.getY());
        ASSERT_EQ(v4.getZ(), v2.getZ());
        ASSERT_EQ(v4.getW(), v2.getW());
    }
    TEST(MatrixTesting, getMatrix)
    {
        Matrix m = Matrix();
        Vector v1 = m.getVector(0);
        Vector v2 = m.getVector(1);
        Vector v3 = m.getVector(2);
        Vector v4 = m.getVector(3);
        checkVector(v1, Vector(1, 0, 0, 0));
        checkVector(v2, Vector(0, 1, 0, 0));
        checkVector(v3, Vector(0, 0, 1, 0));
        checkVector(v4, Vector(0, 0, 0, 1));
    }
    TEST(MatrixTesting, getOutRangeMatrix)
    {
        Matrix m = Matrix();
        ASSERT_THROW(m.getVector(-1), GeneralException);
        ASSERT_THROW(m.getVector(4), GeneralException);
    }
    TEST(MatrixTesting, copyCtor)
    {
        Matrix m = Matrix();
        Matrix m1 = Matrix(m);
        checkVector(m1.getVector(0), Vector(1, 0, 0, 0));
        checkVector(m1.getVector(1), Vector(0, 1, 0, 0));
        checkVector(m1.getVector(2), Vector(0, 0, 1, 0));
        checkVector(m1.getVector(3), Vector(0, 0, 0, 1));
    }
    TEST(MatrixTesting, addition)
    {
        Matrix m = Matrix();
        Matrix m1 = Matrix(m);
        checkVector(m1.getVector(0), Vector(1, 0, 0, 0));
        checkVector(m1.getVector(1), Vector(0, 1, 0, 0));
        checkVector(m1.getVector(2), Vector(0, 0, 1, 0));
        checkVector(m1.getVector(3), Vector(0, 0, 0, 1));
        Matrix m2 = m + m1;
        checkVector(m2.getVector(0), Vector(2, 0, 0, 0));
        checkVector(m2.getVector(1), Vector(0, 2, 0, 0));
        checkVector(m2.getVector(2), Vector(0, 0, 2, 0));
        checkVector(m2.getVector(3), Vector(0, 0, 0, 2));
    }
    TEST(MatrixTesting, translate)
    {
        Matrix m1 = Matrix();
        m1 = m1.translation(5, 2, 8);
        checkVector(m1.getVector(0), Vector(1, 0, 0, 0));
        checkVector(m1.getVector(1), Vector(0, 1, 0, 0));
        checkVector(m1.getVector(2), Vector(0, 0, 1, 0));
        checkVector(m1.getVector(3), Vector(5, 2, 8, 1));
        Vector trans, empty;
        m1.getComp(trans, empty, empty);
        checkVector(trans, Vector(5, 2, 8));
        m1 = m1.translation(Vector(4, 7, 1));
        checkVector(m1.getVector(0), Vector(1, 0, 0, 0));
        checkVector(m1.getVector(1), Vector(0, 1, 0, 0));
        checkVector(m1.getVector(2), Vector(0, 0, 1, 0));
        checkVector(m1.getVector(3), Vector(9, 9, 9, 1));
        m1.getComp(trans, empty, empty);
        checkVector(trans, Vector(9, 9, 9));
    }
    TEST(MatrixTesting, scale)
    {
        Matrix m1 = Matrix();
        m1 = m1.scale(5, 2, 8);
        checkVector(m1.getVector(0), Vector(5, 0, 0, 0));
        checkVector(m1.getVector(1), Vector(0, 2, 0, 0));
        checkVector(m1.getVector(2), Vector(0, 0, 8, 0));
        checkVector(m1.getVector(3), Vector(0, 0, 0, 1));
        Vector scal, empty;
        m1.getComp(empty, scal, empty);
        checkVector(scal, Vector(5, 2, 8, 0));
    }
    TEST(MatrixTesting, MatrixCtor)
    {
        float mat[4][4];
        for (int i = 0; i < 16; i++)
        {
            mat[i / 4][i % 4] = i;
        }
        Matrix m2 = Matrix(mat);
        checkVector(m2.getVector(0), Vector(0, 1, 2, 3));
        checkVector(m2.getVector(1), Vector(4, 5, 6, 7));
        checkVector(m2.getVector(2), Vector(8, 9, 10, 11));
        checkVector(m2.getVector(3), Vector(12, 13, 14, 15));
    }
    TEST(MatrixTesting, transpose)
    {
        float mat[4][4];
        for (int i = 0; i < 16; i++)
        {
            mat[i / 4][i % 4] = i;
        }
        Matrix m2 = Matrix(mat);
        for (int i = 0; i < 16; i++)
        {
            mat[i % 4][i / 4] = i;
        }
        Matrix m3 = Matrix(mat);
        checkVector(m2.getVector(0), Vector(0, 1, 2, 3));
        checkVector(m2.getVector(1), Vector(4, 5, 6, 7));
        checkVector(m2.getVector(2), Vector(8, 9, 10, 11));
        checkVector(m2.getVector(3), Vector(12, 13, 14, 15));
        m2 = m2.transpose();
        checkVector(m2.getVector(0), m3.getVector(0));
        checkVector(m2.getVector(1), m3.getVector(1));
        checkVector(m2.getVector(2), m3.getVector(2));
        checkVector(m2.getVector(3), m3.getVector(3));
    }
    TEST(MatrixTesting, copy)
    {
        Matrix m1 = Matrix();
        m1 = m1.translation(5, 2, 8);
        checkVector(m1.getVector(0), Vector(1, 0, 0, 0));
        checkVector(m1.getVector(1), Vector(0, 1, 0, 0));
        checkVector(m1.getVector(2), Vector(0, 0, 1, 0));
        checkVector(m1.getVector(3), Vector(5, 2, 8, 1));
        m1 = m1.translation(Vector(4, 7, 1));
        checkVector(m1.getVector(0), Vector(1, 0, 0, 0));
        checkVector(m1.getVector(1), Vector(0, 1, 0, 0));
        checkVector(m1.getVector(2), Vector(0, 0, 1, 0));
        checkVector(m1.getVector(3), Vector(9, 9, 9, 1));
        Matrix m3 = m1;
        checkVector(m3.getVector(0), Vector(1, 0, 0, 0));
        checkVector(m3.getVector(1), Vector(0, 1, 0, 0));
        checkVector(m3.getVector(2), Vector(0, 0, 1, 0));
        checkVector(m3.getVector(3), Vector(9, 9, 9, 1));
    }
    TEST(MatrixTesting, Multiple)
    {
        float mat[4][4];
        for (int i = 0; i < 16; i++)
        {
            mat[i / 4][i % 4] = i;
        }
        Matrix m2 = Matrix(mat);
        for (size_t i = 0; i < 16; i++)
        {
            mat[i / 4][i % 4] = i / 4 + i % 4;
        }
        Matrix m4 = Matrix(mat);
        Matrix m5 = m2 * m4;
        checkVector(m5.getVector(0), Vector(14, 20, 26, 32));
        checkVector(m5.getVector(1), Vector(38, 60, 82, 104));
        checkVector(m5.getVector(2), Vector(62, 100, 138, 176));
        checkVector(m5.getVector(3), Vector(86, 140, 194, 248));
        Matrix m1 = Matrix();
        m1 = m1.translation(5, 2, 8);
        checkVector(m1.getVector(0), Vector(1, 0, 0, 0));
        checkVector(m1.getVector(1), Vector(0, 1, 0, 0));
        checkVector(m1.getVector(2), Vector(0, 0, 1, 0));
        checkVector(m1.getVector(3), Vector(5, 2, 8, 1));
        m1 = m1.translation(Vector(4, 7, 1));
        checkVector(m1.getVector(0), Vector(1, 0, 0, 0));
        checkVector(m1.getVector(1), Vector(0, 1, 0, 0));
        checkVector(m1.getVector(2), Vector(0, 0, 1, 0));
        checkVector(m1.getVector(3), Vector(9, 9, 9, 1));
        Matrix m3 = m1;
        checkVector(m3.getVector(0), Vector(1, 0, 0, 0));
        checkVector(m3.getVector(1), Vector(0, 1, 0, 0));
        checkVector(m3.getVector(2), Vector(0, 0, 1, 0));
        checkVector(m3.getVector(3), Vector(9, 9, 9, 1));
        m5 = m5 * m3;
        checkVector(m5.getVector(0), Vector(302, 308, 314, 32));
        checkVector(m5.getVector(1), Vector(974, 996, 1018, 104));
        checkVector(m5.getVector(2), Vector(1646, 1684, 1722, 176));
        checkVector(m5.getVector(3), Vector(2318, 2372, 2426, 248));
    }
    TEST(MatrixTesting, divide)
    {
        float matf[4][4];
        for (int i = 0; i < 16; i++)
        {
            matf[i / 4][i % 4] = i + 1;
        }
        Matrix mat = Matrix(matf);
        float di[4][4];
        di[0][0] = di[1][2] = di[2][2] = di[3][3] = 1;
        di[1][0] = di[0][1] = di[2][3] = di[3][1] = 2;
        di[0][2] = di[1][1] = di[2][0] = di[3][2] = 3;
        di[0][3] = di[1][3] = di[2][1] = di[3][0] = 4;
        Matrix div = Matrix(di);
        Matrix res = mat / div;
        checkVector(res.getVector(0), Vector(1, 0, 0, 0));
        checkVector(res.getVector(1), Vector(5 / 3.0, 0, 2 / 5.0, 8 / 15.0));
        checkVector(res.getVector(2), Vector(7 / 3.0, 0, 4 / 5.0, 16 / 15.0));
        checkVector(res.getVector(3), Vector(3, 0, 6 / 5.0, 8 / 5.0));
    }
    TEST(MatrixTesting, rotationAxis)
    {
        Matrix m6;
        m6 = m6.rotationX(33 * 3.14 / 180.0);
        checkVector(m6.getVector(0), Vector(1, 0, 0, 0));
        ASSERT_EQ(0.0f, m6.getVector(1).getX());
        ASSERT_NEAR(0.83867f, m6.getVector(1).getY(), 0.001f);
        ASSERT_NEAR(0.54463f, m6.getVector(1).getZ(), 0.001f);
        ASSERT_EQ(0.0f, m6.getVector(1).getW());
        ASSERT_EQ(0.0f, m6.getVector(2).getX());
        ASSERT_NEAR(0.83867f, m6.getVector(2).getZ(), 0.001f);
        ASSERT_NEAR(-0.54463f, m6.getVector(2).getY(), 0.001f);
        ASSERT_EQ(0.0f, m6.getVector(2).getW());
        checkVector(m6.getVector(3), Vector(0, 0, 0, 1));
        Vector rot, empty;
        m6.getComp(empty, empty, rot);
        //checkVector(rot, Vector(33 * 3.14 / 180.0, 0, 0));//TODO
        Matrix m6a;
        m6a = m6a.rotationXYZ(33 * 3.14 / 180.0, 0.0, 0.0);
        checkVector(m6.getVector(0), m6a.getVector(0));
        checkVector(m6.getVector(1), m6a.getVector(1));
        checkVector(m6.getVector(2), m6a.getVector(2));
        checkVector(m6.getVector(3), m6a.getVector(3));
        Matrix m7;
        m7 = m7.rotationY(48 * 3.14 / 180.0);
        checkVector(m7.getVector(1), Vector(0, 1, 0, 0));
        ASSERT_NEAR(0.66913f, m7.getVector(0).getX(), 0.001f);
        ASSERT_EQ(0.0f, m7.getVector(0).getY());
        ASSERT_NEAR(-0.74314f, m7.getVector(0).getZ(), 0.001f);
        ASSERT_EQ(0.0f, m7.getVector(0).getW());
        ASSERT_NEAR(0.74314f, m7.getVector(2).getX(), 0.001f);
        ASSERT_NEAR(0.66913f, m7.getVector(2).getZ(), 0.001f);
        ASSERT_EQ(0.0f, m7.getVector(2).getY());
        ASSERT_EQ(0.0f, m7.getVector(2).getW());
        checkVector(m7.getVector(3), Vector(0, 0, 0, 1));
        m7.getComp(empty, empty, rot);
        //checkVector(rot, Vector(0, 48 * 3.14 / 180.0, 0));//TODO
        Matrix m7a;
        m7a = m7a.rotationXYZ(0.0, 48 * 3.14 / 180.0, 0.0);
        checkVector(m7.getVector(0), m7a.getVector(0));
        checkVector(m7.getVector(1), m7a.getVector(1));
        checkVector(m7.getVector(2), m7a.getVector(2));
        checkVector(m7.getVector(3), m7a.getVector(3));
        Matrix m8;
        m8 = m8.rotationZ(11 * 3.14 / 180.0);
        checkVector(m8.getVector(2), Vector(0, 0, 1, 0));
        ASSERT_NEAR(0.98162f, m8.getVector(0).getX(), 0.001f);
        ASSERT_NEAR(0.19081f, m8.getVector(0).getY(), 0.001f);
        ASSERT_EQ(0.0f, m8.getVector(0).getZ());
        ASSERT_EQ(0.0f, m8.getVector(0).getW());
        ASSERT_NEAR(-0.19081f, m8.getVector(1).getX(), 0.001f);
        ASSERT_NEAR(0.98162f, m8.getVector(1).getY(), 0.001f);
        ASSERT_EQ(0.0f, m8.getVector(1).getZ());
        ASSERT_EQ(0.0f, m8.getVector(1).getW());
        checkVector(m8.getVector(3), Vector(0, 0, 0, 1));
        m8.getComp(empty, empty, rot);
        //checkVector(rot, Vector(0, 0, 11 * 3.14 / 180.0));//TODO
        Matrix m8a;
        m8a = m8a.rotationXYZ(0.0, 0.0, 11 * 3.14 / 180.0);
        checkVector(m8.getVector(0), m8a.getVector(0));
        checkVector(m8.getVector(1), m8a.getVector(1));
        checkVector(m8.getVector(2), m8a.getVector(2));
        checkVector(m8.getVector(3), m8a.getVector(3));
    }
    TEST(MatrixTesting, rotation)
    {
        Matrix m6;
        m6 = m6.rotationX(33 * 3.14 / 180.0);
        Matrix m7;
        m7 = m7.rotationY(48 * 3.14 / 180.0);
        Matrix m8;
        m8 = m8.rotationZ(11 * 3.14 / 180.0);
        Matrix m9 = m8 * m6*m7;
        Matrix m10;
        m10 = m10.rotationXYZ(Vector(33 * 3.14 / 180.0, 48 * 3.14 / 180.0, 11 * 3.14 / 180.0));
        Matrix m11;
        m11 = m11.rotationXYZ(33 * 3.14 / 180.0, 48 * 3.14 / 180.0, 11 * 3.14 / 180.0);
        checkVector(m10.getVector(0), m11.getVector(0));
        checkVector(m10.getVector(1), m11.getVector(1));
        checkVector(m10.getVector(2), m11.getVector(2));
        checkVector(m10.getVector(3), m11.getVector(3));
        checkVector(m9.getVector(0), m10.getVector(0));
        checkVector(m9.getVector(1), m10.getVector(1));
        checkVector(m9.getVector(2), m10.getVector(2));
        checkVector(m9.getVector(3), m10.getVector(3));
    }
    TEST(MatrixTesting, getTranslation)
    {
        Matrix m;
        Matrix t, s, r;
        t = t.translation(631, 243, 537);
        s = s.scale(2, 5, 3);
        Matrix x, y, z;
        x = x.rotationX(2.4);
        y = y.rotationY(0.1);
        z = z.rotationZ(1.7);
        Matrix rot = x * y * z;
        r = r.rotationXYZ(2.4, 0.1, 1.7);
        m = s * r*t;
        Vector trans, scal, rotv;
        m.getComp(trans, scal, rotv);
        checkVector(Vector(631, 243, 537), trans);
        checkVector(Vector(2, 5, 3, 0), scal);
        //checkVector(Vector(2.4, 0.1, 1.7, 0), rotv);//TODO
    }
    static void checkVector(Vector &v1, Vector &v2)
    {
        const float DIFFRENCE = 0.00001;
        ASSERT_NEAR(v1.getX(), v2.getX(), DIFFRENCE);
        ASSERT_NEAR(v1.getY(), v2.getY(), DIFFRENCE);
        ASSERT_NEAR(v1.getZ(), v2.getZ(), DIFFRENCE);
    }
}