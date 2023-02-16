add_rules("mode.debug", "mode.release")
add_requires("ffmpeg")

target("qt_ffmpeg")
    add_rules("qt.widgetapp")
    add_packages("ffmpeg")

    add_frameworks("QtMultimedia")

    add_headerfiles("Headers/**.h")
    add_files("./main.cpp")
    add_files("code/**.cpp")

    add_files("ui/ffpemg_player.ui")
    add_files("ui/ffmpeg.qrc")

    -- add files with Q_OBJECT meta (only for qt.moc)
    add_files("Headers/mainwindow.h")
