(function ($) {
  jQuery(document).ready(function ($) {
    $("#action").change(function (e) {
      var actions = {
        "move": {
          "forward": "Forward",
          "backward": "Backward",
          "left": "Left",
          "right": "Right",
        },
        "sound": {
          "sound1": "sound1"
        }
      };

      var action = null;
      if ($.inArray($(this).val(), actions)) {
        action = actions[$(this).val()];
      }
      $("#sub-action-group").css("display", ["inherit", "none"][1 * (action == null)]);

      if (action != null) {
        var options = ["<option selected disabled>-- Select action --</option>"];
        for (var option in action) {
          options.push("<option value='" + option + "'>" + action[option] + "</option>");
        }
        $("#sub-action").html(options.join(""));
      }
    });


    setInterval(function () {
        $("#preview").attr("src", "/image?" + Date.now())
    }, 400);

  })
})(jQuery);
