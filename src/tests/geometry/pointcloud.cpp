/**
 * Copyright (c) 2020 Neka-Nat
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
**/
#include "cupoch/geometry/pointcloud.h"

#include <gtest/gtest.h>
#include <thrust/unique.h>

#include "cupoch/geometry/boundingvolume.h"
#include "tests/test_utility/unit_test.h"

using namespace Eigen;
using namespace cupoch;
using namespace cupoch::geometry;
using namespace unit_test;

TEST(PointCloud, Constuctor) {
    geometry::PointCloud pc;

    EXPECT_EQ(3, pc.Dimension());

    // public member variables
    EXPECT_EQ(0u, pc.points_.size());
    EXPECT_EQ(0u, pc.normals_.size());
    EXPECT_EQ(0u, pc.colors_.size());

    // public members
    EXPECT_TRUE(pc.IsEmpty());

    ExpectEQ(Zero3f, pc.GetMinBound());
    ExpectEQ(Zero3f, pc.GetMaxBound());

    EXPECT_FALSE(pc.HasPoints());
    EXPECT_FALSE(pc.HasNormals());
    EXPECT_FALSE(pc.HasColors());
}

TEST(PointCloud, Clear) {
    int size = 100;

    Vector3f vmin(0.0, 0.0, 0.0);
    Vector3f vmax(1000.0, 1000.0, 1000.0);

    geometry::PointCloud pc;

    thrust::host_vector<Vector3f> points(size);
    Rand(points, vmin, vmax, 0);
    thrust::host_vector<Vector3f> normals(size);
    Rand(normals, vmin, vmax, 0);
    thrust::host_vector<Vector3f> colors(size);
    Rand(colors, vmin, vmax, 0);
    pc.SetPoints(points);
    pc.SetNormals(normals);
    pc.SetColors(colors);

    ExpectEQ(Vector3f(19.607843, 0.0, 0.0), pc.GetMinBound());
    ExpectEQ(Vector3f(996.078431, 996.078431, 996.078431), pc.GetMaxBound());

    EXPECT_FALSE(pc.IsEmpty());
    EXPECT_TRUE(pc.HasPoints());
    EXPECT_TRUE(pc.HasNormals());
    EXPECT_TRUE(pc.HasColors());

    pc.Clear();

    // public members
    EXPECT_TRUE(pc.IsEmpty());

    ExpectEQ(Zero3f, pc.GetMinBound());
    ExpectEQ(Zero3f, pc.GetMaxBound());

    EXPECT_FALSE(pc.HasPoints());
    EXPECT_FALSE(pc.HasNormals());
    EXPECT_FALSE(pc.HasColors());
}

TEST(PointCloud, IsEmpty) {
    int size = 100;

    Vector3f vmin(0.0, 0.0, 0.0);
    Vector3f vmax(1000.0, 1000.0, 1000.0);

    geometry::PointCloud pc;

    EXPECT_TRUE(pc.IsEmpty());

    thrust::host_vector<Vector3f> points(size);
    Rand(points, vmin, vmax, 0);
    pc.SetPoints(points);

    EXPECT_FALSE(pc.IsEmpty());
}

TEST(PointCloud, GetMinBound) {
    int size = 100;

    Vector3f vmin(0.0, 0.0, 0.0);
    Vector3f vmax(1000.0, 1000.0, 1000.0);

    geometry::PointCloud pc;

    thrust::host_vector<Vector3f> points(size);
    Rand(points, vmin, vmax, 0);
    pc.SetPoints(points);

    ExpectEQ(Vector3f(19.607843, 0.0, 0.0), pc.GetMinBound());
    ExpectEQ(Vector3f(19.607843, 0.0, 0.0), pc.GetMinBound());
}

TEST(PointCloud, GetMaxBound) {
    int size = 100;

    Vector3f vmin(0.0, 0.0, 0.0);
    Vector3f vmax(1000.0, 1000.0, 1000.0);

    geometry::PointCloud pc;

    thrust::host_vector<Vector3f> points(size);
    Rand(points, vmin, vmax, 0);
    pc.SetPoints(points);

    ExpectEQ(Vector3f(996.078431, 996.078431, 996.078431), pc.GetMaxBound());
    ExpectEQ(Vector3f(996.078431, 996.078431, 996.078431), pc.GetMaxBound());
}

TEST(PointCloud, Transform) {
    thrust::host_vector<Vector3f> ref_points;
    ref_points.push_back(Vector3f(1.411252, 4.274168, 3.130918));
    ref_points.push_back(Vector3f(1.231757, 4.154505, 3.183678));
    ref_points.push_back(Vector3f(1.403168, 4.268779, 2.121679));
    ref_points.push_back(Vector3f(1.456767, 4.304511, 2.640845));
    ref_points.push_back(Vector3f(1.620902, 4.413935, 1.851255));
    ref_points.push_back(Vector3f(1.374684, 4.249790, 3.062485));
    ref_points.push_back(Vector3f(1.328160, 4.218773, 1.795728));
    ref_points.push_back(Vector3f(1.713446, 4.475631, 1.860145));
    ref_points.push_back(Vector3f(1.409239, 4.272826, 2.011462));
    ref_points.push_back(Vector3f(1.480169, 4.320113, 1.177780));

    thrust::host_vector<Vector3f> ref_normals;
    ref_normals.push_back(Vector3f(396.470588, 1201.176471, 880.352941));
    ref_normals.push_back(Vector3f(320.392157, 1081.176471, 829.019608));
    ref_normals.push_back(Vector3f(268.627451, 817.647059, 406.666667));
    ref_normals.push_back(Vector3f(338.431373, 1000.392157, 614.117647));
    ref_normals.push_back(Vector3f(423.137255, 1152.549020, 483.607843));
    ref_normals.push_back(Vector3f(432.549020, 1337.647059, 964.392157));
    ref_normals.push_back(Vector3f(139.607843, 443.921569, 189.176471));
    ref_normals.push_back(Vector3f(291.764706, 762.352941, 317.058824));
    ref_normals.push_back(Vector3f(134.117647, 407.058824, 191.882353));
    ref_normals.push_back(Vector3f(274.509804, 801.568627, 218.627451));

    int size = 10;
    geometry::PointCloud pc;

    Vector3f vmin(0.0, 0.0, 0.0);
    Vector3f vmax(1000.0, 1000.0, 1000.0);

    thrust::host_vector<Vector3f> points(size);
    Rand(points, vmin, vmax, 0);
    pc.SetPoints(points);

    thrust::host_vector<Vector3f> normals(size);
    normals.resize(size);
    Rand(normals, vmin, vmax, 0);
    pc.SetNormals(normals);

    Matrix4f transformation;
    transformation << 0.10, 0.20, 0.30, 0.40, 0.50, 0.60, 0.70, 0.80, 0.90,
            0.10, 0.11, 0.12, 0.13, 0.14, 0.15, 0.16;

    pc.Transform(transformation);

    ExpectEQ(ref_points, pc.GetPoints());
    ExpectEQ(ref_normals, pc.GetNormals());
}

TEST(PointCloud, HasPoints) {
    int size = 100;

    geometry::PointCloud pc;

    EXPECT_FALSE(pc.HasPoints());

    thrust::host_vector<Vector3f> points(size);
    pc.SetPoints(points);

    EXPECT_TRUE(pc.HasPoints());
}

TEST(PointCloud, HasNormals) {
    int size = 100;

    geometry::PointCloud pc;

    EXPECT_FALSE(pc.HasNormals());

    thrust::host_vector<Vector3f> points(size);
    pc.SetPoints(points);
    thrust::host_vector<Vector3f> normals(size);
    pc.SetNormals(normals);

    EXPECT_TRUE(pc.HasNormals());
}

TEST(PointCloud, HasColors) {
    int size = 100;

    geometry::PointCloud pc;

    EXPECT_FALSE(pc.HasColors());

    thrust::host_vector<Vector3f> points(size);
    pc.SetPoints(points);
    thrust::host_vector<Vector3f> colors(size);
    pc.SetColors(colors);

    EXPECT_TRUE(pc.HasColors());
}

TEST(PointCloud, NormalizeNormals) {
    thrust::host_vector<Vector3f> ref;
    ref.push_back(Vector3f(0.692861, 0.323767, 0.644296));
    ref.push_back(Vector3f(0.650010, 0.742869, 0.160101));
    ref.push_back(Vector3f(0.379563, 0.870761, 0.312581));
    ref.push_back(Vector3f(0.575046, 0.493479, 0.652534));
    ref.push_back(Vector3f(0.320665, 0.448241, 0.834418));
    ref.push_back(Vector3f(0.691127, 0.480526, 0.539850));
    ref.push_back(Vector3f(0.227557, 0.973437, 0.025284));
    ref.push_back(Vector3f(0.281666, 0.156994, 0.946582));
    ref.push_back(Vector3f(0.341869, 0.894118, 0.289273));
    ref.push_back(Vector3f(0.103335, 0.972118, 0.210498));
    ref.push_back(Vector3f(0.441745, 0.723783, 0.530094));
    ref.push_back(Vector3f(0.336903, 0.727710, 0.597441));
    ref.push_back(Vector3f(0.434917, 0.862876, 0.257471));
    ref.push_back(Vector3f(0.636619, 0.435239, 0.636619));
    ref.push_back(Vector3f(0.393717, 0.876213, 0.277918));
    ref.push_back(Vector3f(0.275051, 0.633543, 0.723167));
    ref.push_back(Vector3f(0.061340, 0.873191, 0.483503));
    ref.push_back(Vector3f(0.118504, 0.276510, 0.953677));
    ref.push_back(Vector3f(0.930383, 0.360677, 0.065578));
    ref.push_back(Vector3f(0.042660, 0.989719, 0.136513));

    int size = 20;

    Vector3f vmin(0.0, 0.0, 0.0);
    Vector3f vmax(1000.0, 1000.0, 1000.0);

    geometry::PointCloud pc;

    thrust::host_vector<Vector3f> normals(size);
    Rand(normals, vmin, vmax, 0);
    pc.SetNormals(normals);

    pc.NormalizeNormals();

    ExpectEQ(ref, pc.GetNormals());
}

TEST(PointCloud, SelectByIndex) {
    thrust::host_vector<Vector3f> ref;
    ref.push_back(Vector3f(796.078431, 909.803922, 196.078431));
    ref.push_back(Vector3f(768.627451, 525.490196, 768.627451));
    ref.push_back(Vector3f(400.000000, 890.196078, 282.352941));
    ref.push_back(Vector3f(349.019608, 803.921569, 917.647059));
    ref.push_back(Vector3f(19.607843, 454.901961, 62.745098));
    ref.push_back(Vector3f(666.666667, 529.411765, 39.215686));
    ref.push_back(Vector3f(164.705882, 439.215686, 878.431373));
    ref.push_back(Vector3f(909.803922, 482.352941, 215.686275));
    ref.push_back(Vector3f(615.686275, 278.431373, 784.313725));
    ref.push_back(Vector3f(415.686275, 168.627451, 905.882353));
    ref.push_back(Vector3f(949.019608, 50.980392, 517.647059));
    ref.push_back(Vector3f(639.215686, 756.862745, 90.196078));
    ref.push_back(Vector3f(203.921569, 886.274510, 121.568627));
    ref.push_back(Vector3f(356.862745, 549.019608, 576.470588));
    ref.push_back(Vector3f(529.411765, 756.862745, 301.960784));
    ref.push_back(Vector3f(992.156863, 576.470588, 874.509804));
    ref.push_back(Vector3f(227.450980, 698.039216, 313.725490));
    ref.push_back(Vector3f(470.588235, 592.156863, 941.176471));
    ref.push_back(Vector3f(431.372549, 0.000000, 341.176471));
    ref.push_back(Vector3f(596.078431, 831.372549, 231.372549));
    ref.push_back(Vector3f(674.509804, 482.352941, 478.431373));
    ref.push_back(Vector3f(694.117647, 670.588235, 635.294118));
    ref.push_back(Vector3f(109.803922, 360.784314, 576.470588));
    ref.push_back(Vector3f(592.156863, 662.745098, 286.274510));
    ref.push_back(Vector3f(823.529412, 329.411765, 184.313725));

    size_t size = 100;
    geometry::PointCloud pc;

    Vector3f vmin(0.0, 0.0, 0.0);
    Vector3f vmax(1000.0, 1000.0, 1000.0);

    thrust::host_vector<Vector3f> points(size);
    Rand(points, vmin, vmax, 0);
    pc.SetPoints(points);

    thrust::host_vector<size_t> indices(size / 4);
    Rand(indices, 0, size, 0);

    // remove duplicates
    thrust::host_vector<size_t>::iterator it;
    it = thrust::unique(indices.begin(), indices.end());
    indices.resize(thrust::distance(indices.begin(), it));

    auto output_pc = pc.SelectByIndex(indices);
    auto output_pt = output_pc->GetPoints();

    sort::Do(ref);
    sort::Do(output_pt);
    ExpectEQ(ref, output_pt);
}

TEST(PointCloud, VoxelDownSample) {
    thrust::host_vector<Vector3f> ref_points;
    ref_points.push_back(Vector3f(19.607843, 454.901961, 62.745098));
    ref_points.push_back(Vector3f(66.666667, 949.019608, 525.490196));
    ref_points.push_back(Vector3f(82.352941, 192.156863, 662.745098));
    ref_points.push_back(Vector3f(105.882353, 996.078431, 215.686275));
    ref_points.push_back(Vector3f(141.176471, 603.921569, 15.686275));
    ref_points.push_back(Vector3f(152.941176, 400.000000, 129.411765));
    ref_points.push_back(Vector3f(239.215686, 133.333333, 803.921569));
    ref_points.push_back(Vector3f(294.117647, 635.294118, 521.568627));
    ref_points.push_back(Vector3f(333.333333, 764.705882, 274.509804));
    ref_points.push_back(Vector3f(349.019608, 803.921569, 917.647059));
    ref_points.push_back(Vector3f(364.705882, 509.803922, 949.019608));
    ref_points.push_back(Vector3f(400.000000, 890.196078, 282.352941));
    ref_points.push_back(Vector3f(490.196078, 972.549020, 290.196078));
    ref_points.push_back(Vector3f(509.803922, 835.294118, 611.764706));
    ref_points.push_back(Vector3f(552.941176, 474.509804, 627.450980));
    ref_points.push_back(Vector3f(768.627451, 525.490196, 768.627451));
    ref_points.push_back(Vector3f(796.078431, 909.803922, 196.078431));
    ref_points.push_back(Vector3f(839.215686, 392.156863, 780.392157));
    ref_points.push_back(Vector3f(890.196078, 345.098039, 62.745098));
    ref_points.push_back(Vector3f(913.725490, 635.294118, 713.725490));

    thrust::host_vector<Vector3f> ref_normals;
    ref_normals.push_back(Vector3f(0.042660, 0.989719, 0.136513));
    ref_normals.push_back(Vector3f(0.061340, 0.873191, 0.483503));
    ref_normals.push_back(Vector3f(0.103335, 0.972118, 0.210498));
    ref_normals.push_back(Vector3f(0.118504, 0.276510, 0.953677));
    ref_normals.push_back(Vector3f(0.227557, 0.973437, 0.025284));
    ref_normals.push_back(Vector3f(0.275051, 0.633543, 0.723167));
    ref_normals.push_back(Vector3f(0.281666, 0.156994, 0.946582));
    ref_normals.push_back(Vector3f(0.320665, 0.448241, 0.834418));
    ref_normals.push_back(Vector3f(0.336903, 0.727710, 0.597441));
    ref_normals.push_back(Vector3f(0.341869, 0.894118, 0.289273));
    ref_normals.push_back(Vector3f(0.379563, 0.870761, 0.312581));
    ref_normals.push_back(Vector3f(0.393717, 0.876213, 0.277918));
    ref_normals.push_back(Vector3f(0.434917, 0.862876, 0.257471));
    ref_normals.push_back(Vector3f(0.441745, 0.723783, 0.530094));
    ref_normals.push_back(Vector3f(0.575046, 0.493479, 0.652534));
    ref_normals.push_back(Vector3f(0.636619, 0.435239, 0.636619));
    ref_normals.push_back(Vector3f(0.650010, 0.742869, 0.160101));
    ref_normals.push_back(Vector3f(0.691127, 0.480526, 0.539850));
    ref_normals.push_back(Vector3f(0.692861, 0.323767, 0.644296));
    ref_normals.push_back(Vector3f(0.930383, 0.360677, 0.065578));

    thrust::host_vector<Vector3f> ref_colors;
    ref_colors.push_back(Vector3f(5.000000, 116.000000, 16.000000));
    ref_colors.push_back(Vector3f(17.000000, 242.000000, 134.000000));
    ref_colors.push_back(Vector3f(21.000000, 49.000000, 169.000000));
    ref_colors.push_back(Vector3f(27.000000, 254.000000, 55.000000));
    ref_colors.push_back(Vector3f(36.000000, 154.000000, 4.000000));
    ref_colors.push_back(Vector3f(39.000000, 102.000000, 33.000000));
    ref_colors.push_back(Vector3f(61.000000, 34.000000, 205.000000));
    ref_colors.push_back(Vector3f(75.000000, 162.000000, 133.000000));
    ref_colors.push_back(Vector3f(85.000000, 195.000000, 70.000000));
    ref_colors.push_back(Vector3f(89.000000, 205.000000, 234.000000));
    ref_colors.push_back(Vector3f(93.000000, 130.000000, 242.000000));
    ref_colors.push_back(Vector3f(102.000000, 227.000000, 72.000000));
    ref_colors.push_back(Vector3f(125.000000, 248.000000, 74.000000));
    ref_colors.push_back(Vector3f(130.000000, 213.000000, 156.000000));
    ref_colors.push_back(Vector3f(141.000000, 121.000000, 160.000000));
    ref_colors.push_back(Vector3f(196.000000, 134.000000, 196.000000));
    ref_colors.push_back(Vector3f(203.000000, 232.000000, 50.000000));
    ref_colors.push_back(Vector3f(214.000000, 100.000000, 199.000000));
    ref_colors.push_back(Vector3f(227.000000, 88.000000, 16.000000));
    ref_colors.push_back(Vector3f(233.000000, 162.000000, 182.000000));

    size_t size = 20;
    geometry::PointCloud pc;

    thrust::host_vector<Vector3f> points(size);
    Rand(points, Zero3f, Vector3f(1000.0, 1000.0, 1000.0), 0);
    pc.SetPoints(points);
    thrust::host_vector<Vector3f> normals(size);
    Rand(normals, Zero3f, Vector3f(10.0, 10.0, 10.0), 0);
    pc.SetNormals(normals);
    thrust::host_vector<Vector3f> colors(size);
    Rand(colors, Zero3f, Vector3f(255.0, 255.0, 255.0), 0);
    pc.SetColors(colors);

    float voxel_size = 0.5;
    auto output_pc = pc.VoxelDownSample(voxel_size);

    // sometimes the order of these Vector3d values can be mixed-up
    // sort these vectors in order to match the expected order.
    auto output_pt = output_pc->GetPoints();
    auto output_nl = output_pc->GetNormals();
    auto output_cl = output_pc->GetColors();
    sort::Do(ref_points);
    sort::Do(ref_normals);
    sort::Do(ref_colors);
    sort::Do(output_pt);
    sort::Do(output_nl);
    sort::Do(output_cl);

    ExpectEQ(ref_points, output_pt);
    ExpectEQ(ref_normals, output_nl);
    ExpectEQ(ref_colors, output_cl);
}

TEST(PointCloud, UniformDownSample) {
    thrust::host_vector<Vector3f> ref;
    ref.push_back(Vector3f(839.215686, 392.156863, 780.392157));
    ref.push_back(Vector3f(364.705882, 509.803922, 949.019608));
    ref.push_back(Vector3f(152.941176, 400.000000, 129.411765));
    ref.push_back(Vector3f(490.196078, 972.549020, 290.196078));
    ref.push_back(Vector3f(66.666667, 949.019608, 525.490196));
    ref.push_back(Vector3f(235.294118, 968.627451, 901.960784));
    ref.push_back(Vector3f(435.294118, 929.411765, 929.411765));
    ref.push_back(Vector3f(827.450980, 329.411765, 227.450980));
    ref.push_back(Vector3f(396.078431, 811.764706, 682.352941));
    ref.push_back(Vector3f(615.686275, 278.431373, 784.313725));
    ref.push_back(Vector3f(101.960784, 125.490196, 494.117647));
    ref.push_back(Vector3f(584.313725, 243.137255, 149.019608));
    ref.push_back(Vector3f(172.549020, 239.215686, 796.078431));
    ref.push_back(Vector3f(66.666667, 203.921569, 458.823529));
    ref.push_back(Vector3f(996.078431, 50.980392, 866.666667));
    ref.push_back(Vector3f(356.862745, 549.019608, 576.470588));
    ref.push_back(Vector3f(745.098039, 627.450980, 35.294118));
    ref.push_back(Vector3f(666.666667, 494.117647, 160.784314));
    ref.push_back(Vector3f(325.490196, 231.372549, 70.588235));
    ref.push_back(Vector3f(470.588235, 592.156863, 941.176471));
    ref.push_back(Vector3f(674.509804, 482.352941, 478.431373));
    ref.push_back(Vector3f(345.098039, 184.313725, 607.843137));
    ref.push_back(Vector3f(529.411765, 86.274510, 258.823529));
    ref.push_back(Vector3f(772.549020, 286.274510, 329.411765));
    ref.push_back(Vector3f(764.705882, 698.039216, 117.647059));

    size_t size = 100;
    geometry::PointCloud pc;

    Vector3f vmin(0.0, 0.0, 0.0);
    Vector3f vmax(1000.0, 1000.0, 1000.0);

    thrust::host_vector<Vector3f> points(size);
    Rand(points, vmin, vmax, 0);
    pc.SetPoints(points);

    size_t every_k_points = 4;
    auto output_pc = pc.UniformDownSample(every_k_points);

    ExpectEQ(ref, output_pc->GetPoints());
}

TEST(PointCloud, CropPointCloud) {
    size_t size = 100;
    geometry::PointCloud pc;

    Vector3f vmin(0.0, 0.0, 0.0);
    Vector3f vmax(1000.0, 1000.0, 1000.0);

    thrust::host_vector<Vector3f> points(size);
    Rand(points, vmin, vmax, 0);
    pc.SetPoints(points);

    Vector3f minBound(200.0, 200.0, 200.0);
    Vector3f maxBound(800.0, 800.0, 800.0);
    auto output_pc =
            pc.Crop(geometry::AxisAlignedBoundingBox(minBound, maxBound));

    ExpectLE(minBound, output_pc->GetPoints());
    ExpectGE(maxBound, output_pc->GetPoints());
}

TEST(PointCloud, EstimateNormals) {
    thrust::host_vector<Vector3f> ref;
    ref.push_back(Vector3f(0.282003, 0.866394, 0.412111));
    ref.push_back(Vector3f(0.550791, 0.829572, -0.091869));
    ref.push_back(Vector3f(0.076085, -0.974168, 0.212620));
    ref.push_back(Vector3f(0.261265, 0.825182, 0.500814));
    ref.push_back(Vector3f(0.035397, 0.428362, 0.902913));
    ref.push_back(Vector3f(0.711421, 0.595291, 0.373508));
    ref.push_back(Vector3f(0.519141, 0.552592, 0.652024));
    ref.push_back(Vector3f(0.490520, 0.573293, -0.656297));
    ref.push_back(Vector3f(0.324029, 0.744177, 0.584128));
    ref.push_back(Vector3f(0.120589, -0.989854, 0.075152));
    ref.push_back(Vector3f(0.370700, 0.767066, 0.523632));
    ref.push_back(Vector3f(0.874692, -0.158725, -0.457952));
    ref.push_back(Vector3f(0.238700, 0.937064, -0.254819));
    ref.push_back(Vector3f(0.518237, 0.540189, 0.663043));
    ref.push_back(Vector3f(0.238700, 0.937064, -0.254819));
    ref.push_back(Vector3f(0.080943, -0.502095, -0.861016));
    ref.push_back(Vector3f(0.753661, -0.527376, -0.392261));
    ref.push_back(Vector3f(0.721099, 0.542859, -0.430489));
    ref.push_back(Vector3f(0.159997, -0.857801, -0.488446));
    ref.push_back(Vector3f(0.445869, 0.725107, 0.524805));
    ref.push_back(Vector3f(0.019474, -0.592041, -0.805672));
    ref.push_back(Vector3f(0.024464, 0.856206, 0.516056));
    ref.push_back(Vector3f(0.478041, 0.869593, -0.123631));
    ref.push_back(Vector3f(0.104534, -0.784980, -0.610638));
    ref.push_back(Vector3f(0.073901, 0.570353, 0.818069));
    ref.push_back(Vector3f(0.178678, 0.974506, 0.135693));
    ref.push_back(Vector3f(0.178678, 0.974506, 0.135693));
    ref.push_back(Vector3f(0.581675, 0.167795, -0.795926));
    ref.push_back(Vector3f(0.069588, -0.845043, -0.530150));
    ref.push_back(Vector3f(0.626448, 0.486534, 0.608973));
    ref.push_back(Vector3f(0.670665, 0.657002, 0.344321));
    ref.push_back(Vector3f(0.588868, 0.011829, 0.808143));
    ref.push_back(Vector3f(0.081974, 0.638039, 0.765628));
    ref.push_back(Vector3f(0.159997, -0.857801, -0.488446));
    ref.push_back(Vector3f(0.559499, 0.824271, -0.086826));
    ref.push_back(Vector3f(0.612885, 0.727999, 0.307229));
    ref.push_back(Vector3f(0.178678, 0.974506, 0.135693));
    ref.push_back(Vector3f(0.268803, 0.796616, 0.541431));
    ref.push_back(Vector3f(0.604933, 0.787776, -0.116044));
    ref.push_back(Vector3f(0.111998, 0.869999, -0.480165));

    size_t size = 40;
    geometry::PointCloud pc;

    Vector3f vmin(0.0, 0.0, 0.0);
    Vector3f vmax(1000.0, 1000.0, 1000.0);

    thrust::host_vector<Vector3f> points(size);
    Rand(points, vmin, vmax, 0);
    pc.SetPoints(points);

    pc.EstimateNormals(geometry::KDTreeSearchParamKNN());
    thrust::host_vector<Vector3f> normals = pc.GetNormals();
    for (size_t idx = 0; idx < ref.size(); ++idx) {
        if ((ref[idx](0) < 0 && normals[idx](0) > 0) ||
            (ref[idx](0) > 0 && normals[idx](0) < 0)) {
            normals[idx] *= -1;
        }
    }
    ExpectEQ(ref, normals);
}

TEST(PointCloud, OrientNormalsToAlignWithDirection) {
    thrust::host_vector<Vector3f> ref;
    ref.push_back(Vector3f(0.282003, 0.866394, 0.412111));
    ref.push_back(Vector3f(0.550791, 0.829572, -0.091869));
    ref.push_back(Vector3f(0.076085, -0.974168, 0.212620));
    ref.push_back(Vector3f(0.261265, 0.825182, 0.500814));
    ref.push_back(Vector3f(0.035397, 0.428362, 0.902913));
    ref.push_back(Vector3f(0.711421, 0.595291, 0.373508));
    ref.push_back(Vector3f(0.519141, 0.552592, 0.652024));
    ref.push_back(Vector3f(-0.490520, -0.573293, 0.656297));
    ref.push_back(Vector3f(0.324029, 0.744177, 0.584128));
    ref.push_back(Vector3f(-0.120589, 0.989854, -0.075152));
    ref.push_back(Vector3f(0.370700, 0.767066, 0.523632));
    ref.push_back(Vector3f(-0.874692, 0.158725, 0.457952));
    ref.push_back(Vector3f(-0.238700, -0.937064, 0.254819));
    ref.push_back(Vector3f(0.518237, 0.540189, 0.663043));
    ref.push_back(Vector3f(-0.238700, -0.937064, 0.254819));
    ref.push_back(Vector3f(-0.080943, 0.502095, 0.861016));
    ref.push_back(Vector3f(-0.753661, 0.527376, 0.392261));
    ref.push_back(Vector3f(-0.721099, -0.542859, 0.430489));
    ref.push_back(Vector3f(-0.159997, 0.857801, 0.488446));
    ref.push_back(Vector3f(0.445869, 0.725107, 0.524805));
    ref.push_back(Vector3f(-0.019474, 0.592041, 0.805672));
    ref.push_back(Vector3f(0.024464, 0.856206, 0.516056));
    ref.push_back(Vector3f(0.478041, 0.869593, -0.123631));
    ref.push_back(Vector3f(-0.104534, 0.784980, 0.610638));
    ref.push_back(Vector3f(0.073901, 0.570353, 0.818069));
    ref.push_back(Vector3f(0.178678, 0.974506, 0.135693));
    ref.push_back(Vector3f(0.178678, 0.974506, 0.135693));
    ref.push_back(Vector3f(-0.581675, -0.167795, 0.795926));
    ref.push_back(Vector3f(-0.069588, 0.845043, 0.530150));
    ref.push_back(Vector3f(0.626448, 0.486534, 0.608973));
    ref.push_back(Vector3f(0.670665, 0.657002, 0.344321));
    ref.push_back(Vector3f(0.588868, 0.011829, 0.808143));
    ref.push_back(Vector3f(0.081974, 0.638039, 0.765628));
    ref.push_back(Vector3f(-0.159997, 0.857801, 0.488446));
    ref.push_back(Vector3f(0.559499, 0.824271, -0.086826));
    ref.push_back(Vector3f(0.612885, 0.727999, 0.307229));
    ref.push_back(Vector3f(0.178678, 0.974506, 0.135693));
    ref.push_back(Vector3f(0.268803, 0.796616, 0.541431));
    ref.push_back(Vector3f(0.604933, 0.787776, -0.116044));
    ref.push_back(Vector3f(-0.111998, -0.869999, 0.480165));

    int size = 40;
    geometry::PointCloud pc;

    Vector3f vmin(0.0, 0.0, 0.0);
    Vector3f vmax(1000.0, 1000.0, 1000.0);

    thrust::host_vector<Vector3f> points;
    points.resize(size);
    Rand(points, vmin, vmax, 0);
    pc.SetPoints(points);

    pc.EstimateNormals();
    pc.OrientNormalsToAlignWithDirection(Vector3f(1.5, 0.5, 3.3));

    ExpectEQ(ref, pc.GetNormals());
}