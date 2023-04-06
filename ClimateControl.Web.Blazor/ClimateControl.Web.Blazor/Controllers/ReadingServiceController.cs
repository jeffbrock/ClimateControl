using Microsoft.AspNetCore.Mvc;
using ClimateControl.Web.Blazor.Services;

namespace ClimateControl.Web.Blazor.Controllers
{
    [ApiController]
    [Route("reading")]
    public class ReadingServiceController : Controller
    {
        private readonly ReadingService _readingService;

        public ReadingServiceController(ReadingService readingService) => _readingService = readingService;

        [HttpPost]
        [Consumes("application/x-www-form-urlencoded")]
        public void Post([FromForm] Reading reading)
        {
            if (reading != null)
            {
                reading.ReadDate = DateTime.UtcNow;
                _readingService.PostReading(reading);
            }
        }
    }
}