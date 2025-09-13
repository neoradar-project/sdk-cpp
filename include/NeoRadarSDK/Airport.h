#pragma once
#include <string>
#include <vector>
#include <optional>

namespace PluginSDK::Airport {

/**
 * @enum AirportStatus
 * @brief Status of an airport (active/observed)
 */
enum class AirportStatus {
    Active = 0,
    Observed = 1
};

/**
 * @enum RunwayType
 * @brief Status of an airport (active/observed)
 */
enum class RunwayType {
    Departure = 0,
    Arrival = 1
};

/**
 * @struct RunwayStatusChange
 */
struct RunwayStatusChange {
    std::string icao;
    std::string runway;
    RunwayType type;
};

/**
 * @struct AirportConfig
 * @brief Configuration for an airport
 */
struct AirportConfig {
    std::string icao;
    std::vector<std::string> arrRunways;
    std::vector<std::string> depRunways;
    AirportStatus status;
};

/**
 * @struct AirportAddedEvent
 * @brief Event fired when an airport is added
 */
struct AirportAddedEvent {
    std::string icao;
};

/**
 * @struct AirportRemovedEvent
 * @brief Event fired when an airport is removed
 */
struct AirportRemovedEvent {
    std::string icao;
};

/**
 * @struct AirportStatusChangedEvent
 * @brief Event fired when an airport's status changes
 */
struct AirportStatusChangedEvent {
    std::string icao;
    AirportStatus status;
};

/**
 * @struct RunwayStatusChangedEvent
 * @brief Event fired when a runway status changes
 */
struct RunwayStatusChangedEvent {
    std::string icao;
    std::string runway;
    bool active;
    bool isArrival;
};

/**
 * @struct AirportConfigurationsUpdatedEvent
 * @brief Event fired when airport configurations are updated
 */
struct AirportConfigurationsUpdatedEvent {
    int configurationCount;
};

class AirportAPI {
public:
    virtual ~AirportAPI() = default;

    // Read operations
    /**
     * @brief Get all airport configurations in the system
     * @return Vector of airport configurations
     */
    virtual std::vector<AirportConfig> getConfigurations() = 0;

    /**
     * @brief Get airport configuration by ICAO code
     * @param icao The ICAO code to look up
     * @return Airport configuration or nullopt if not found
     */
    virtual std::optional<AirportConfig> getConfigurationByIcao(const std::string& icao) = 0;

    /**
     * @brief Check if a departure runway is active for an airport
     * @param icao The airport ICAO code
     * @param runway The runway identifier
     * @return True if the departure runway is active, false otherwise
     */
    virtual bool isDepRunwayActive(const std::string& icao, const std::string& runway) = 0;

    /**
     * @brief Check if an arrival runway is active for an airport
     * @param icao The airport ICAO code
     * @param runway The runway identifier
     * @return True if the arrival runway is active, false otherwise
     */
    virtual bool isArrRunwayActive(const std::string& icao, const std::string& runway) = 0;

    // Write operations
    /**
     * @brief Sets the status of a given runway for an airport
     * @param icao The airport ICAO code
     * @param runway The runway identifier
     * @param type The configuration type of this runway (dep/arr)
     * @return True if operation was successful, false otherwise
     */
    virtual bool setRunwayStatus(const std::string& icao, const std::string& runway, const RunwayType type) = 0;

    /**
     * @brief Shows/hides runway centerlines
     * @param icao The airport ICAO code
     * @param show Whether to show or hide runway centerlines
     * @return True if operation was successful, false otherwise
     */
    virtual bool setShowRunwayCenterlines(const std::string& icao, bool show) = 0;

    /**
     * @brief Removes the status of a given runway for an airport
     * @param icao The airport ICAO code
     * @param runway The runway identifier
     * @param type The configuration type of this runway (dep/arr)
     * @return True if operation was successful, false otherwise
     */
    virtual bool removeRunwayStatus(const std::string& icao, const std::string& runway, const RunwayType type) = 0;


    /**
         * @brief Batch update runway statuses with additions and removals
         * @param toAdd Vector of runway status changes to add
         * @param toRemove Vector of runway status changes to remove
         * @return Tuple of (successful additions, failed additions, successful removals, failed removals)
     */
    virtual std::tuple<std::vector<RunwayStatusChange>, std::vector<RunwayStatusChange>,
        std::vector<RunwayStatusChange>, std::vector<RunwayStatusChange>>
    batchUpdateRunways(const std::vector<RunwayStatusChange>& toAdd,
        const std::vector<RunwayStatusChange>& toRemove) = 0;

    /**
     * @brief Remove an airport from the system
     * @param icao The airport ICAO code
     * @return True if operation was successful, false otherwise
     */
    virtual bool deleteAirport(const std::string& icao) = 0;

    /**
     * @brief Set the status of an airport
     * @param icao The airport ICAO code
     * @param status The new airport status
     * @return True if operation was successful, false otherwise
     */
    virtual bool setAirportStatus(const std::string& icao, AirportStatus status) = 0;
};

} // namespace PluginSDK::Airport