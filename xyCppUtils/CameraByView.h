/**
  * A simple camera represented by its view point, camera direction (from view
  * point), distance to view point and up direction. This class is handy for
  * interactions in 3D graphics viewer (e.g. OpenGL).
  *
  * Author: Ying Xiong.
  * Created: Jun 15, 2013.
  */

#ifndef __XYUTILS_CAMERA_BY_VIEW_H__
#define __XYUTILS_CAMERA_BY_VIEW_H__

#include <Eigen/Core>

#include "Quaternion.h"

namespace xyUtils  {
class CameraByView {
 public:
  // Constructor.
  CameraByView(Eigen::Vector3d lookat, Eigen::Vector3d cam_direc, double cam_dist,
               Eigen::Vector3d up)
      : lookat_(lookat), cam_direc_(cam_direc), cam_dist_(cam_dist), up_(up) { }
  // Set the 'lookat' point.
  void SetLookat(Eigen::Vector3d lookat) { lookat_ = lookat;  }
  // Get the 'lookat' point.
  const Eigen::Vector3d& GetLookat() const { return lookat_;  }
  // Get the 'up' direction.
  const Eigen::Vector3d& GetUp() const { return up_;  }
  // Get the camera's 3D location (i.e. camera center).
  Eigen::Vector3d GetCamPosition() const {
    return lookat_ + cam_dist_*cam_direc_;
  }
  // Set camera distance to the object.
  void SetCamDist(double dist) { cam_dist_ = dist; }
  // Zoom in (scale < 1) or out (scale > 1).
  void Zoom(double scale) { cam_dist_ *= scale;  }
  // Translate the camera in right and up direction.
  void TranslateRightUp(double r, double u) {
    Eigen::Vector3d right;
    Quaternion::VectCross(up_.data(), cam_direc_.data(), right.data());
    lookat_ += (r*right + u*up_);
  }
  // Rotate the camera direction for 'ang' (radian) across up direction.
  void RotateAcrossUp(double ang) {
    Quaternion::RotateVectByAxisAngle(cam_direc_.data(), up_.data(), ang,
                                      cam_direc_.data());
  }
  // Rotate the camera direction for 'ang' (radian) across right direction.
  void RotateAcrossRight(double ang) {
    Eigen::Vector3d right;
    Quaternion::VectCross(up_.data(), cam_direc_.data(), right.data());
    Quaternion::RotateVectByAxisAngle(cam_direc_.data(), right.data(), ang,
                                      cam_direc_.data());
    NormalizeUp();
  }
 private:
  // Normalize the up direction to be orthogonal to 'cam_direc_' and 'up_'.
  void NormalizeUp() {
    up_ -= cam_direc_.dot(up_) * cam_direc_;
    up_.normalize();
  }
  Eigen::Vector3d lookat_;    // The view point this camera looks at.
  Eigen::Vector3d cam_direc_; // The camera direction, starting from the view point.
  double cam_dist_;           // The distance between camera and the view point.
  Eigen::Vector3d up_;        // The up direction of the camera.
};
}   // namespace xyUtils

#endif   // __XYUTILS_CAMERA_BY_VIEW_H__
