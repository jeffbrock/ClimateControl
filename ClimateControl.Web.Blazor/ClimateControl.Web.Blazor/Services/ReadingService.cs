using System.Text.Json;

namespace ClimateControl.Web.Blazor.Services
{
    public class Reading
    {
        public DateTime? ReadDate { get; set; }
        public decimal TemperatureFahrenheit { get; set; }
        public decimal HumidityPercent { get; set; }
    }

    public class ReadingService
    {
        private readonly string _readingsJson;

        public ReadingService(IConfiguration config) => _readingsJson = config["ReadingsJson"];

        public Reading[]? GetReadings() => JsonSerializer.Deserialize<Reading[]>(File.ReadAllText(_readingsJson));

        public void PostReading(Reading reading)
        {
            var data = JsonSerializer.Deserialize<Reading[]>(File.ReadAllText(_readingsJson))!.ToList();
            data.Add(reading);
            File.WriteAllText(_readingsJson, JsonSerializer.Serialize(data));
        }
    }
}