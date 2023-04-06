using Microsoft.AspNetCore.Mvc;
using ClimateControl.Web.Blazor.Services;

namespace ClimateControl.Web.Blazor.Controllers
{
    [ApiController]
    [Route("settings")]
    public class SettingsController : Controller
    {
        private readonly SettingsService _settingsService;

        public SettingsController(SettingsService settingsService) => _settingsService = settingsService;

        [HttpGet]
        public Settings? Get() => _settingsService.GetSettings();        
    }
}
