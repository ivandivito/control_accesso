package pulser.app


enum class BeaconPresenceState(val tag: String) {
    PRESENT("present"),
    MISSING("missing"),
    LOST("lost");

    companion object {
        private val map = BeaconPresenceState.values().associateBy(BeaconPresenceState::tag)
        fun fromTag(tag: String) = map[tag]
    }
}

enum class AccessControlLed(val id: Int) {
    LED_RGB(0),
    LED_1(1),
    LED_2(2),
    LED_3(3)
}

enum class AccessControlMode(val id: Int) {
    PRESENCE_MODE(0),
    DISTANCE_MODE(1)
}

data class BeaconAddr(val major: Int = -1,val minor:Int = -1)

data class BeaconState(val addr: BeaconAddr,val presence: BeaconPresenceState,val distance: Int)

data class BeaconControlMode(val addr: BeaconAddr,val led: AccessControlLed,val mode: AccessControlMode)

private val BEACON_STATE_REGEX = """report: addr: ([0-9]+) ([0-9]+), presence: ([a-z]+), last distance: ([0-9]+)""".toRegex()

//TODO revisar
fun String.toBeaconState() : BeaconState{
    val data = BEACON_STATE_REGEX.matchEntire(this)?.groups
    return data.let {
        BeaconState(
                BeaconAddr(
                        data?.get(1)?.value?.toIntOrNull() ?: -1,
                        data?.get(2)?.value?.toIntOrNull() ?: -1),
                data?.get(2)?.value?.let { BeaconPresenceState.fromTag(it) } ?: BeaconPresenceState.MISSING,
                data?.get(2)?.value?.toIntOrNull() ?: -1
        )
    }
}

