#include "common.h"
#include "config.h"
#include <libconfig.h>

#include <stdio.h>
#include <stdlib.h>
const char *sf_path;

int fetchConfig()
{
    config_t cfg;
    config_setting_t *setting;

    config_init(&cfg);

    if (!config_read_file(&cfg, "config.cfg"))
    {
        fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),
                config_error_line(&cfg), config_error_text(&cfg));
        config_destroy(&cfg);
        return (EXIT_FAILURE);
    }
    if (config_lookup_string(&cfg, "path", &sf_path))
        printf("\n - Soundfont path: %s\n\n", sf_path);
    else
        fprintf(stderr, "No 'path' setting in configuration file.\n");

    return 0;
}

void writeConfig()
{
    static const char *output_file = "newconfig.cfg";
    config_t cfg;
    config_setting_t *root, *setting, *group, *array;
    int i;

    config_init(&cfg);
    root = config_root_setting(&cfg);

    /* Add some settings to the configuration. */
    group = config_setting_add(root, "address", CONFIG_TYPE_GROUP);

    setting = config_setting_add(group, "street", CONFIG_TYPE_STRING);
    config_setting_set_string(setting, "1 Woz Way");

    setting = config_setting_add(group, "city", CONFIG_TYPE_STRING);
    config_setting_set_string(setting, "San Jose");

    setting = config_setting_add(group, "state", CONFIG_TYPE_STRING);
    config_setting_set_string(setting, "CA");

    setting = config_setting_add(group, "zip", CONFIG_TYPE_INT);
    config_setting_set_int(setting, 95110);

    array = config_setting_add(root, "numbers", CONFIG_TYPE_ARRAY);

    for (i = 0; i < 10; ++i)
    {
        setting = config_setting_add(array, NULL, CONFIG_TYPE_INT);
        config_setting_set_int(setting, 10 * i);
    }

    /* Write out the new configuration. */
    if (!config_write_file(&cfg, output_file))
    {
        fprintf(stderr, "Error while writing file.\n");
        config_destroy(&cfg);
    }

    fprintf(stderr, "New configuration successfully written to: %s\n",
            output_file);

    config_destroy(&cfg);
}