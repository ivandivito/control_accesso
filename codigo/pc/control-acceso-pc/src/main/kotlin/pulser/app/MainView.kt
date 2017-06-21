package pulser.app

import javafx.geometry.Insets
import javafx.geometry.Pos
import javafx.scene.control.ComboBox
import javafx.scene.control.ListCell
import javafx.scene.control.SelectionMode
import javafx.scene.control.TextField
import javafx.scene.layout.BorderPane
import javafx.scene.layout.Priority
import javafx.scene.paint.Color
import javafx.scene.text.Text
import javafx.scene.text.TextFlow
import rx.javafx.kt.actionEvents
import rx.javafx.kt.observeOnFx
import tornadofx.*

class MainView : View() {
    override val root = BorderPane()

    var console: TextFlow by singleAssign()

    var readBeaconAddrMajorField : TextField by singleAssign()
    var readBeaconAddrMinorField : TextField by singleAssign()

    var setBeaconControlMajorField : TextField by singleAssign()
    var setBeaconControlMinorField : TextField by singleAssign()
    var setBeaconControlLed = AccessControlLed.LED_RGB
    var setBeaconControlMode = AccessControlMode.PRESENCE_MODE

    val accessControlLedList = enumValues<AccessControlLed>().toList().observable()
    val accessControlModeList = enumValues<AccessControlMode>().toList().observable()

    init {

        title = "Access Control"

        with(root) {

            prefWidth = 1200.0

            top = menubar {

                menu("File") {

                    item("Connect") {

                        setOnAction {

                            find(PortDialog::class).openModal()

                        }

                    }

                }

            }

            center = form {

                vbox(20) {

                    fieldset("General Commands") {

                        field("Ble Scan") {
                            button("Start: \$S") {
                                actionEvents().map { Unit }.subscribe(Channels.requestStartBeaconScan)
                            }

                            button("Stop: \$E") {
                                actionEvents().map { Unit }.subscribe(Channels.requestStopBeaconScan)
                            }
                        }

                        field("Beacon Database") {
                            button("Delete: \$D") {
                                actionEvents().map { Unit }.subscribe(Channels.requestDeleteBeaconDatabase)
                            }
                        }

                    }

                    fieldset("Read Beacon Database") {
                        alignment = Pos.CENTER

                        field("Major Addr") {
                            textfield {
                                readBeaconAddrMajorField = this
                            }
                        }

                        field("Minor Addr") {
                            textfield {
                                readBeaconAddrMinorField = this
                            }
                        }

                        button("Read: \$R") {
                            spacing = 10.0
                            actionEvents().map {
                                BeaconAddr(readBeaconAddrMajorField.text.toIntOrNull() ?: -1,
                                        readBeaconAddrMinorField.text.toIntOrNull() ?: -1)
                            }.subscribe(Channels.requestBeaconState)
                        }
                    }

                    fieldset("Set Beacon Control Mode") {

                        alignment = Pos.CENTER

                        field("Major Addr") {
                            textfield {
                                setBeaconControlMajorField = this
                            }
                        }

                        field("Minor Addr") {
                            textfield {
                                setBeaconControlMinorField = this
                            }
                        }

                        hbox (30) {

                            alignment = Pos.CENTER

                            label("Led")

                            listview(accessControlLedList) {

                                maxHeight = 100.0

                                setCellFactory {
                                    object: ListCell<AccessControlLed>() {
                                        override fun updateItem(item: AccessControlLed?, empty: Boolean) {
                                            super.updateItem(item, empty)
                                            if (item != null) {
                                                graphic = text(item.name)
                                            }
                                        }
                                    }
                                }

                                onUserSelect(1) {
                                    setBeaconControlLed = it
                                }

                                selectionModel.selectionMode = SelectionMode.SINGLE

                            }

                            label("Control")

                            listview(accessControlModeList) {

                                maxHeight = 100.0

                                setCellFactory {
                                    object : ListCell<AccessControlMode>() {
                                        override fun updateItem(item: AccessControlMode?, empty: Boolean) {
                                            super.updateItem(item, empty)
                                            if (item != null) {
                                                graphic = text(item.name)
                                            }
                                        }
                                    }
                                }

                                onUserSelect(1) {
                                    setBeaconControlMode = it
                                }

                                selectionModel.selectionMode = SelectionMode.SINGLE

                            }

                        }

                        button("Set") {
                            spacing = 10.0
                            actionEvents().map {
                                BeaconControlMode(BeaconAddr(setBeaconControlMajorField.text.toIntOrNull() ?: -1,
                                        setBeaconControlMinorField.text.toIntOrNull() ?: -1),
                                        setBeaconControlLed,
                                        setBeaconControlMode)
                            }.subscribe(Channels.setBeaconControlMode)
                        }
                    }

                }

            }

            right = vbox {

                minWidth = 500.0

                scrollpane {

                    style = "-fx-background: white;"

                    prefHeight = 200.0

                    vboxConstraints {
                        vgrow = Priority.ALWAYS
                    }

                    console = textflow {


                        style = "-fx-font-family: monospace;"

                        Channels.usbOutputLine.observeOnFx().subscribe {
                            appendText("OUT :$it\n", Color.BLUE)
                            vvalue = 1.0
                        }

                        Channels.usbInputLine.observeOnFx().subscribe {
                            appendText("IN  :$it\n", Color.GREEN)
                            vvalue = 1.0
                        }

                        Controller.activeSerialPort.subscribe {
                            appendText(
                                    if (it == null)
                                        "PORT: Disconnected\n"
                                    else
                                        "PORT: Connected to ${it.descriptivePortName}\n",
                                    Color.BLACK)
                            vvalue = 1.0
                        }

                    }

                }

                hbox {

                    alignment = Pos.CENTER

                    label("Command: ") {
                        padding = Insets(2.0)
                    }

                    val command = textfield {

                        style = "-fx-font-family: monospace"

                        hboxConstraints {
                            hgrow = Priority.ALWAYS
                        }

                        actionEvents().map { text }.subscribe(Channels.usbOutputLine)

                        Channels.usbOutputLine.subscribe { clear() }

                    }

                    button("Send") {

                        actionEvents().map { command.text }.subscribe(Channels.usbOutputLine)

                    }

                    button("Clear") {

                        setOnAction {
                            console.clear()
                        }

                    }

                }

            }

        }

    }

}


fun colorText(string: String, color: Color): Text {
    return Text().apply {
        text = string
        fill = color
    }
}

fun TextFlow.appendText(string: String, color: Color = Color.BLACK) {
    children.add(colorText(string, color))
}