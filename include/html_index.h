const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>CloneLeaf</title>
    <script src="https://unpkg.com/vue"></script>
    <script src="https://unpkg.com/axios/dist/axios.min.js"></script>
</head>

<body>
    <div id="app">

        <h1>{{ message }}</h1><br>
        <label for="mode">Mode</label>
        <select id="modesList" v-model="settings.mode">
            <option v-for="mode in modesList" :key="mode" style="text-decoration: dotted;">{{mode}}</option>
        </select>
        <br>
        <span v-if="settings.mode==='Solid'">
            <label for="panel">Panel</label>
            <select id="panel" v-model="selectedPanel">
                <option v-for="(panel,index) in panels">{{index}}</option>
            </select>
            <label for="colorInput">Color</label>
            <input v-if="selectedPanel" v-model="panels[selectedPanel]" type="color">
        </span>
        <br>

        <br>
        <label for="speed">Speed</label>
        <input id="speed" type="range" min="10" max="100" v-model="settings.speed">
        <label for="speed">{{settings.speed}}</label><br>
        <label for="brightness">Brightness</label>
        <input id="brightness" type="range" min="0" max="255" v-model="settings.brightness">
        <label for="brightness">{{settings.brightness}}</label>
        <br>
        <br>
        <br>
        <button v-if="edited" @click="save">Save</button>
    </div>

    <script>
        var app = new Vue({
            el: '#app',
            data: {
                message: 'Hello CloneLeaf!',
                selectedPanel: 0,
                selectedLED: 0,
                settings: {
                    speed: 10,
                    brightness: 70,
                    mode: "Dot",
                },
                modesList: [
                    "Rainbow", "Rainbow2", "Dot"
                ],
                edited: false,
                panels: []
            },
            watch: {
                settings: {
                    handler() {
                        this.edited = true;
                    }, deep: true
                }
            },
            async beforeCreate() {
                await fetch("/getsettings").then((response) => {
                    return response.json()
                }).then((data) => {
                    this.settings.speed = data.speed;
                    this.settings.brightness = data.brightness;
                    this.settings.mode = data.mode;
                });
            },
            methods: {
                async save() {
                    this.edited = false;
                    axios.post("/setsettings", this.settings).then(response => {
                        console.log(response);
                    }).catch(error => {
                        console.error(error);
                    })
                }
            },
        })
    </script>
</body>

</html>
)=====";