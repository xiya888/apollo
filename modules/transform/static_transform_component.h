/******************************************************************************
 * Copyright 2018 The Apollo Authors. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/

#pragma once

#include <memory>
#include <string>
#include <vector>

#include "modules/localization/proto/localization.pb.h"
#include "modules/transform/proto/static_transform_conf.pb.h"
#include "modules/transform/proto/transform.pb.h"

#include "cyber/component/component.h"
namespace apollo {
namespace transform {

class StaticTransformComponent final : public apollo::cyber::Component<> {
 public:
  StaticTransformComponent() = default;
  ~StaticTransformComponent() = default;

 public:
  bool Init() override;

 private:
  void SendTransforms();
  void SendTransform(const std::vector<TransformStamped>& msgtf);
  bool ParseFromYaml(const std::string& file_path, TransformStamped* transform);

  apollo::static_transform::Conf conf_;
  std::shared_ptr<cyber::Writer<TransformStampeds>> writer_;
  TransformStampeds transform_stampeds_;

  // Since the recorded data packet does not contain tf_static channel data, the
  // timestamp during conversion is incorrect, and the corresponding conversion
  // parameter matrix cannot be queried.
  double location_timestamp_ = 0.0;
  int send_message_number_ = 0;
  std::shared_ptr<cyber::Reader<apollo::localization::LocalizationEstimate>>
      localization_reader_;
  /**
   * @brief Get the realtime localization pose time for transform.
   * @param localization contains the vehicle message in ENU coordinate system.
   */
  void LocalizationInfo(
      const std::shared_ptr<apollo::localization::LocalizationEstimate>&
          localization);
  /**
   * @brief Fill in the timestamp and other content to the module.
   * @param module_name is the module name that could set.
   * @param msg is the proto content that needs to be set.
   */
  void NewFillHeader(const std::string& module_name, TransformStampeds* msg);
};

CYBER_REGISTER_COMPONENT(StaticTransformComponent)

}  // namespace transform
}  // namespace apollo
