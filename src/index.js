var Module = {
  onRuntimeInitialized: function() {
    document.getElementById('hi')
      .addEventListener('click', Module.print_event_target_value)
  }
};

