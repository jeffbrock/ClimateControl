using System.Text.Json;

namespace ClimateControl.Web.Blazor.Services
{
    public class Settings
    {
        public decimal SetTemperatureFahrenheit { get; set; }
        public decimal SetHumidityPercent { get; set; }
        public int ReadIntervalMinutes { get; set; }
    }

    public class SettingsService
    {
        private readonly string _settingsJson;

        public SettingsService(IConfiguration config) => _settingsJson = config["SettingsJson"];

        public Settings? GetSettings() => JsonSerializer.Deserialize<Settings>(File.ReadAllText(_settingsJson));

        public void SaveSettings(Settings settings) => File.WriteAllText(_settingsJson, JsonSerializer.Serialize(settings));
    }
}