// Copyright (c) 2017 Computer Vision Center (CVC) at the Universitat Autonoma
// de Barcelona (UAB).
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#pragma once

#include "carla/PythonUtil.h"
#include "carla/client/World.h"
#include "carla/client/detail/Simulator.h"

namespace carla {
namespace client {

  class Client {
  public:

    /// Construct a carla client.
    ///
    /// @param host IP address of the host machine running the simulator.
    /// @param port TCP port to connect with the simulator.
    /// @param worker_threads number of asynchronous threads to use, or 0 to use
    ///        all available hardware concurrency.
    explicit Client(
        const std::string &host,
        uint16_t port,
        size_t worker_threads = 0u);

    /// Set a timeout for networking operations. If set, any networking
    /// operation taking longer than @a timeout throws rpc::timeout.
    void SetTimeout(time_duration timeout) {
      _simulator->SetNetworkingTimeout(timeout);
    }

    /// Return the version string of this client API.
    std::string GetClientVersion() const {
      return _simulator->GetClientVersion();
    }

    /// Return the version string of the simulator we are connected to.
    std::string GetServerVersion() const {
      return _simulator->GetServerVersion();
    }

    std::vector<std::string> GetAvailableMaps() const {
      return _simulator->GetAvailableMaps();
    }

    World ReloadWorld() const {
      return World{_simulator->ReloadEpisode()};
    }

    World LoadWorld(std::string map_name) const {
      return World{_simulator->LoadEpisode(std::move(map_name))};
    }

    /// Return an instance of the world currently active in the simulator.
    World GetWorld() const {
      return World{_simulator->GetCurrentEpisode()};
    }

    std::string StartRecorder(std::string name) {
      return _simulator->StartRecorder(name);
    }

    void StopRecorder(void) {
      _simulator->StopRecorder();
    }

    std::string ShowRecorderFileInfo(std::string name) {
      return _simulator->ShowRecorderFileInfo(name);
    }

    std::string ShowRecorderCollisions(std::string name, char type1, char type2) {
      return _simulator->ShowRecorderCollisions(name, type1, type2);
    }

    std::string ShowRecorderActorsBlocked(std::string name, double min_time, double min_distance) {
      return _simulator->ShowRecorderActorsBlocked(name, min_time, min_distance);
    }

    std::string ReplayFile(std::string name, double start, double duration, uint32_t follow_id) {
      return _simulator->ReplayFile(name, start, duration, follow_id);
    }

    void ApplyBatch(std::vector<rpc::Command> commands, bool do_tick_cue = false) const {
      _simulator->ApplyBatch(std::move(commands), do_tick_cue);
    }

  private:

    std::shared_ptr<detail::Simulator> _simulator;
  };

  inline Client::Client(
      const std::string &host,
      uint16_t port,
      size_t worker_threads)
    : _simulator(
        new detail::Simulator(host, port, worker_threads),
        PythonUtil::ReleaseGILDeleter()) {}

} // namespace client
} // namespace carla
