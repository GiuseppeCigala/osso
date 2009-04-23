////////////////////////////////////////
//  File      : device.cpp            //
//  Written by: g_cigala@virgilio.it  //
//  Copyright : GPL                   //
////////////////////////////////////////

#include "device.h"

////////////////////////////////////////

Device::Device(QWidget * parent) : QWidget(parent)
{
    init();
}

Device::~Device()
{}

void Device::init()
{
    int mixer_fd = get_mixer_fd();
    int mixer_dev = get_mixer_dev(mixer_fd);
    create_extensions(mixer_fd, mixer_dev);
}

int Device::get_mixer_fd()
{
    int mixer_fd = -1;
    const char *dev_mixer;
    mode_t mode = S_IRWXU;

    if ((dev_mixer = getenv("OSS_MIXERDEV")) == NULL)
        dev_mixer = "/dev/mixer";

    if ((mixer_fd = open(dev_mixer, O_RDWR, mode)) == -1)
    {
        switch (errno)
        {
        case ENXIO:
        case ENODEV:
            qCritical() << "Open Sound System is not running in your system.\n";
            break;

        case ENOENT:
            qCritical() << "No mixer device available in your system.\n";
            break;
        }
        exit (-1);
    }
    return mixer_fd;
}

int Device::get_mixer_dev(int mixer_fd)
{
    oss_sysinfo sysinfo;
    oss_mixerinfo mi;
    int default_mix = -1;
    int best_pri = -2;

    if (ioctl (mixer_fd, SNDCTL_SYSINFO, &sysinfo) == -1)
    {
        if (errno == ENXIO)
        {
            qCritical() << "OSS has not detected any supported sound hardware in your system.\n";
            exit (-1);
        }
        if (errno == EINVAL)
        {
            qCritical() << "Error: OSS version 4.1 or later is required.\n";
            exit (-1);
        }
    }

    for (int i = 0; i < sysinfo.nummixers; i++)
    {
        mi.dev = i;

        if (ioctl (mixer_fd, SNDCTL_MIXERINFO, &mi) == -1)
        {
            qCritical() << "SNDCTL_MIXERINFO Error";
            continue;
        }
        if (mi.priority < -1)	/* Not suitable default mixer */
            continue;

        if (mi.priority > best_pri)
        {
            default_mix = i;
            best_pri = mi.priority;
        }
    }
    qDebug() << "Mixer device:" << default_mix << "seems to be the most probable motherboard device";

    return default_mix;
}

QHash <int, Extension *> Device::get_extensions()
{
    return ext_list;
}

QMap <QString, QStringList> Device::get_system_info()
{
    oss_sysinfo sysinfo;
    oss_mixerinfo mi;
    int mixer_fd = get_mixer_fd();
    int mixer_dev = get_mixer_dev(mixer_fd);

    mi.dev = mixer_dev;

    if (ioctl (mixer_fd, SNDCTL_MIXERINFO, &mi) == -1)
    {
        if (errno == ENXIO || errno == ENODEV)
        {
            qCritical() << "Mixer Device not available\n";
            exit (-1);
        }
    }
    if (ioctl (mixer_fd, SNDCTL_SYSINFO, &sysinfo) == -1)
    {
        if (errno == ENXIO)
        {
            qCritical() << "OSS has not detected any supported sound hardware in your system.\n";
            exit (-1);
        }
        if (errno == EINVAL)
        {
            qCritical() << "Error: OSS version 4.0 or later is required.\n";
            exit (-1);
        }
    }
    system_info.insert(tr("1. Mixer device:"), QStringList(QString("%1").arg(mi.name)));
    system_info.insert(tr("2. Product:"), QStringList(QString("%1").arg(sysinfo.product)));
    system_info.insert(tr("3. Version:"), QStringList(QString("%1").arg(sysinfo.version)));
    system_info.insert(tr("4. Version num:"), QStringList(QString("%1").arg(sysinfo.versionnum)));
    system_info.insert(tr("5. License:"), QStringList(QString("%1").arg(sysinfo.license)));

    // get audio device files
    QStringList audio_devices;
    int n = sysinfo.numaudios;

    for (int i = 0; i < n; i++)
    {
        oss_audioinfo ainfo;
        ainfo.dev = i;

        if (ioctl (mixer_fd, SNDCTL_AUDIOINFO, &ainfo) == -1)
        {
            qCritical() << "Can't get device info for /dev/dsp:" << i << "\n";
            continue;
        }
        audio_devices << QString(ainfo.name).append(",").append(ainfo.devnode);
    }
    system_info.insert(tr("6. Audio Device files:"), audio_devices);

    // get audio engines
    QStringList audio_engines;
    n = sysinfo.numaudioengines;

    for (int i = 0; i < n; i++)
    {
        oss_audioinfo ainfo;
        ainfo.dev = i;

        if (ioctl (mixer_fd, SNDCTL_ENGINEINFO, &ainfo) == -1)
        {
            qCritical() << "Can't get device info for /dev/dsp:" << i << "\n";
            continue;
        }
        audio_engines << QString(ainfo.name).append(",").append(ainfo.devnode);
    }
    system_info.insert(tr("7. Audio Engines:"), audio_engines);

    // get sound cards
    QStringList sound_cards;
    n = sysinfo.numcards;

    for (int i = 0; i < sysinfo.numcards; i++)
    {
        oss_card_info cinfo;
        cinfo.card = i;

        if (ioctl (mixer_fd, SNDCTL_CARDINFO, &cinfo) == -1)
        {
            qCritical() << "SNDCTL_CARDINFO Error \n";
            exit (-1);
        }
        sound_cards << QString(cinfo.shortname).append(",").append(cinfo.longname);
    }
    system_info.insert(tr("8. Sound Cards:"), sound_cards);

    return system_info;
}

void Device::create_extensions(int mixer_fd, int mixer_dev)
{
    int i = 0;
    int n = 0;
    int nn = 0;
    oss_mixext ext;
    int marker_seen = 0;

    if (ioctl (mixer_fd, SNDCTL_MIX_NREXT, &n) == -1)
    {
        qCritical() << "SNDCTL_MIX_NREXT Error: OSS version 4.0 or later is required";
        exit (-1);
    }
    else
    {
        qDebug() << "Mixer controls available:" << n;
    }

    for (i = 0; i < n; i++)
    {
        ext.dev = mixer_dev;
        ext.ctrl = i;

        if (ioctl (mixer_fd, SNDCTL_MIX_EXTINFO, &ext) == -1)
        {
            qCritical() << "SNDCTL_MIX_EXTINFO failed: bad OSS version.";
            exit (-1);
        }

        if (ext.type == MIXT_MARKER)
        {
            marker_seen = 1;
            continue;
        }
        else if (!marker_seen)
            continue;

        // create the new Extension
        Extension *ex = new Extension(mixer_dev, mixer_fd, ext);
        ext_list.insert(ext.ctrl, ex); // save the extension

        nn++;
    }

    qDebug() << "Controls accessible by this program:" << nn;
}




