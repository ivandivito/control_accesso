package pulser.app

import tornadofx.*

class PulserApp : App(MainView::class) {

    init {
        //addStageIcon(Image("/app/tornado-fx-logo.png"))
    }

    override fun stop() {
        super.stop()

        Controller.setActiveSerialPort(null)

    }
}
