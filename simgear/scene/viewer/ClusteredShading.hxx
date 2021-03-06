// Copyright (C) 2018  Fernando García Liñán <fernandogarcialinan@gmail.com>
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301, USA

#ifndef SG_CLUSTERED_SHADING_HXX
#define SG_CLUSTERED_SHADING_HXX

#include <atomic>

#include <osg/Camera>
#include <osg/Uniform>

#include <simgear/scene/model/SGLight.hxx>

namespace simgear {
namespace compositor {

class ClusteredShading : public osg::Referenced {
public:
    ClusteredShading(osg::Camera *camera, const SGPropertyNode *config);
    ~ClusteredShading();

    void update(const SGLightList &light_list);
protected:
    // We could make use of osg::Polytope, but it does a lot of std::vector
    // push_back() calls, so we make our own frustum structure for huge
    // performance gains.
    struct Subfrustum {
        osg::Vec4f plane[6];
    };

    struct PointlightBound {
        SGLight *light = nullptr;
        osg::Vec4f position;
        float range = 0.0f;
    };
    struct SpotlightBound {
        SGLight *light = nullptr;
        osg::Vec4f position;
        osg::Vec4f direction;
        float cos_cutoff = 0.0f;
        struct {
            osg::Vec4f center;
            float radius = 0.0f;
        } bounding_sphere;
    };

    void threadFunc(int thread_id);
    void assignLightsToSlice(int slice);
    void writePointlightData();
    void writeSpotlightData();
    float getDepthForSlice(int slice) const;

    osg::observer_ptr<osg::Camera>  _camera;

    osg::ref_ptr<osg::Uniform>      _slice_scale;
    osg::ref_ptr<osg::Uniform>      _slice_bias;
    osg::ref_ptr<osg::Uniform>      _horizontal_tiles;
    osg::ref_ptr<osg::Uniform>      _vertical_tiles;

    int                             _max_pointlights = 0;
    int                             _max_spotlights = 0;
    int                             _max_light_indices = 0;
    int                             _tile_size = 0;
    int                             _depth_slices = 0;
    int                             _num_threads = 0;
    int                             _slices_per_thread = 0;
    int                             _slices_remainder = 0;

    float                           _zNear = 0.0f;
    float                           _zFar = 0.0f;

    int                             _old_width = 0;
    int                             _old_height = 0;

    int                             _n_htiles = 0;
    int                             _n_vtiles = 0;

    float                           _x_step = 0.0f;
    float                           _y_step = 0.0f;

    osg::ref_ptr<osg::Image>        _clusters;
    osg::ref_ptr<osg::Image>        _indices;
    osg::ref_ptr<osg::Image>        _pointlights;
    osg::ref_ptr<osg::Image>        _spotlights;

    std::unique_ptr<Subfrustum[]>   _subfrusta;

    std::vector<PointlightBound>    _point_bounds;
    std::vector<SpotlightBound>     _spot_bounds;

    std::atomic<int>                _global_light_count;
};

} // namespace compositor
} // namespace simgear

#endif /* SG_CLUSTERED_SHADING_HXX */
