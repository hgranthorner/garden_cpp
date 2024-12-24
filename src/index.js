var Module = {
  onRuntimeInitialized: function() {
    document.getElementById('hi')
      .addEventListener('click', () => Module.print(JSON.stringify({ foo: "bar" })))
  }
};

