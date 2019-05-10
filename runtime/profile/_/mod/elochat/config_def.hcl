config {
    net = {
        type = "section"
        options = {
            use_network = true
            constantly_connect = true
            filter_chat = false

            chat_idle_delay {
                default = 250
                min = 1
                max = 10000
            }

            server_domain = "elochat.nori.moe"
        }
    }
}
