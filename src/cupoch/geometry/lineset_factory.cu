#include <Eigen/Dense>

#include "cupoch/geometry/boundingvolume.h"
#include "cupoch/geometry/lineset.h"
#include "cupoch/geometry/pointcloud.h"
#include "cupoch/geometry/trianglemesh.h"
#include "cupoch/utility/platform.h"

using namespace cupoch;
using namespace cupoch::geometry;

namespace {

struct convert_trianglemesh_line_functor {
    convert_trianglemesh_line_functor(const Eigen::Vector3i* triangles, Eigen::Vector2i* lines)
        : triangles_(triangles), lines_(lines){};
    const Eigen::Vector3i* triangles_;
    Eigen::Vector2i* lines_;
    __device__
    void operator() (size_t idx) const {
        const Eigen::Vector3i& vidx = triangles_[idx];
        thrust::minimum<int> min;
        thrust::maximum<int> max;
        lines_[3 * idx] = Eigen::Vector2i(min(vidx[0], vidx[1]), max(vidx[0], vidx[1]));
        lines_[3 * idx + 1] = Eigen::Vector2i(min(vidx[1], vidx[2]), max(vidx[1], vidx[2]));
        lines_[3 * idx + 2] = Eigen::Vector2i(min(vidx[2], vidx[0]), max(vidx[2], vidx[0]));
    }
};

}

std::shared_ptr<LineSet> LineSet::CreateFromPointCloudCorrespondences(
        const PointCloud &cloud0,
        const PointCloud &cloud1,
        const utility::device_vector<thrust::pair<int, int>> &correspondences) {
    auto lineset_ptr = std::make_shared<LineSet>();
    const size_t point0_size = cloud0.points_.size();
    const size_t point1_size = cloud1.points_.size();
    const size_t corr_size = correspondences.size();
    lineset_ptr->points_.resize(point0_size + point1_size);
    lineset_ptr->lines_.resize(corr_size);
    thrust::copy_n(utility::exec_policy(utility::GetStream(0))->on(utility::GetStream(0)),
                   cloud0.points_.begin(), point0_size, lineset_ptr->points_.begin());
    thrust::copy_n(utility::exec_policy(utility::GetStream(1))->on(utility::GetStream(1)),
                   cloud1.points_.begin(), point1_size, lineset_ptr->points_.begin() + point0_size);
    thrust::transform(utility::exec_policy(utility::GetStream(2))->on(utility::GetStream(2)),
                      correspondences.begin(), correspondences.end(),
                      lineset_ptr->lines_.begin(),
                      [=] __device__ (const thrust::pair<int, int>& corrs) {return Eigen::Vector2i(corrs.first, point0_size + corrs.second);});
    cudaSafeCall(cudaDeviceSynchronize());
    return lineset_ptr;
}

std::shared_ptr<LineSet> LineSet::CreateFromTriangleMesh(
        const TriangleMesh &mesh) {
    auto lineset_ptr = std::make_shared<LineSet>();
    lineset_ptr->points_.resize(mesh.vertices_.size());
    lineset_ptr->lines_.resize(mesh.triangles_.size() * 3);
    convert_trianglemesh_line_functor func(thrust::raw_pointer_cast(mesh.triangles_.data()),
                                           thrust::raw_pointer_cast(lineset_ptr->lines_.data()));
    thrust::copy(utility::exec_policy(utility::GetStream(0))->on(utility::GetStream(0)), mesh.vertices_.begin(),
                 mesh.vertices_.end(), lineset_ptr->points_.begin());
    thrust::for_each(utility::exec_policy(utility::GetStream(1))->on(utility::GetStream(1)),
                     thrust::make_counting_iterator<size_t>(0),
                     thrust::make_counting_iterator(mesh.triangles_.size()), func);
    auto end = thrust::unique(utility::exec_policy(utility::GetStream(1))->on(utility::GetStream(1)),
                              lineset_ptr->lines_.begin(), lineset_ptr->lines_.end());
    lineset_ptr->lines_.resize(thrust::distance(lineset_ptr->lines_.begin(), end));
    cudaSafeCall(cudaDeviceSynchronize());
    return lineset_ptr;
}

std::shared_ptr<LineSet> LineSet::CreateFromOrientedBoundingBox(
        const OrientedBoundingBox &box) {
    auto line_set = std::make_shared<LineSet>();
    const auto points = box.GetBoxPoints();
    for (const auto& pt: points) line_set->points_.push_back(pt);
    line_set->lines_.push_back(Eigen::Vector2i(0, 1));
    line_set->lines_.push_back(Eigen::Vector2i(1, 7));
    line_set->lines_.push_back(Eigen::Vector2i(7, 2));
    line_set->lines_.push_back(Eigen::Vector2i(2, 0));
    line_set->lines_.push_back(Eigen::Vector2i(3, 6));
    line_set->lines_.push_back(Eigen::Vector2i(6, 4));
    line_set->lines_.push_back(Eigen::Vector2i(4, 5));
    line_set->lines_.push_back(Eigen::Vector2i(5, 3));
    line_set->lines_.push_back(Eigen::Vector2i(0, 3));
    line_set->lines_.push_back(Eigen::Vector2i(1, 6));
    line_set->lines_.push_back(Eigen::Vector2i(7, 4));
    line_set->lines_.push_back(Eigen::Vector2i(2, 5));
    line_set->PaintUniformColor(box.color_);
    return line_set;
}

std::shared_ptr<LineSet> LineSet::CreateFromAxisAlignedBoundingBox(
        const AxisAlignedBoundingBox &box) {
    auto line_set = std::make_shared<LineSet>();
    const auto points = box.GetBoxPoints();
    for (const auto& pt: points) line_set->points_.push_back(pt);
    line_set->lines_.push_back(Eigen::Vector2i(0, 1));
    line_set->lines_.push_back(Eigen::Vector2i(1, 7));
    line_set->lines_.push_back(Eigen::Vector2i(7, 2));
    line_set->lines_.push_back(Eigen::Vector2i(2, 0));
    line_set->lines_.push_back(Eigen::Vector2i(3, 6));
    line_set->lines_.push_back(Eigen::Vector2i(6, 4));
    line_set->lines_.push_back(Eigen::Vector2i(4, 5));
    line_set->lines_.push_back(Eigen::Vector2i(5, 3));
    line_set->lines_.push_back(Eigen::Vector2i(0, 3));
    line_set->lines_.push_back(Eigen::Vector2i(1, 6));
    line_set->lines_.push_back(Eigen::Vector2i(7, 4));
    line_set->lines_.push_back(Eigen::Vector2i(2, 5));
    line_set->PaintUniformColor(box.color_);
    return line_set;
}