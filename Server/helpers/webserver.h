const char empty_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
    <html>
    <head>
        <title>ESP SERVER</title>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1">

        <link rel="preconnect" href="https://fonts.googleapis.com">
        <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
        <link href="https://fonts.googleapis.com/css2?family=Bebas+Neue&display=swap" rel="stylesheet">
    </head>
    <body>
        Witaj!
    </body>
  </html>
)rawliteral";

const char index_html[] PROGMEM = R"rawliteral(
  <!DOCTYPE HTML>
  <html>
  <head>
      <title>ESP SERVER</title>
      <meta charset="UTF-8">
      <meta name="viewport" content="width=device-width, initial-scale=1">
      <meta http-equiv="refresh" content="10">
  
      <link rel="preconnect" href="https://fonts.googleapis.com">
      <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
      <link href="https://fonts.googleapis.com/css2?family=Bebas+Neue&display=swap" rel="stylesheet">
      <style>
          body, html {
              box-sizing: border-box;
              margin: 0;
              background: #2b2b2b;
              padding: 0;
              width: 100%;
              height: 100%;
              font-family: 'Bebas Neue', cursive;
          }
  
          #content {
              display: flex;
              gap: 15px;
              flex-direction: column;
              padding: 15px;
          }
  
          .actions {
              word-spacing: 100vw;
          }
  
          .actions, .sensors {
              display: flex;
              gap: 15px;
              line-height: 1;
              flex-wrap: wrap;
          }
  
          .actions .action {
              flex-grow: 1;
              flex-shrink: 1;
              flex-basis: 0;
              text-decoration: none;
              padding: 20px;
              background: black;
              color: white;
              font-size: 50px;
              display: flex;
              align-items: center;
              gap: 15px;
          }
  
          .actions .action.blue {
              background: #1cb5c9;
          }
  
          .actions .action.red {
              background: #c91c41;
          }
  
          .actions .action form {
              display: flex;
              gap: 5px;
          }
  
          .sensor {
              padding: 15px 35px;
              flex-direction: column;
              background: white;
              display: flex;
              justify-content: center;
              line-height: 1;
              white-space: nowrap;
              flex-grow: 1;
              flex-shrink: 1;
              flex-basis: 0;
          }
  
          .sensor span {
              font-size: 25px;
          }
  
          .sensor strong {
              font-size: 45px;
          }
      </style>
  </head>
  <body>
  <div id="content">
    <div class="sensors">
        <div class="sensor">
            <span>DEVICE ID</span>
            <strong>%s</strong>
        </div>
        <div class="sensor">
            <span>Soil Temperature</span>
            <strong>%s°C</strong>
        </div>
        <div class="sensor">
            <span>Soil Humidity</span>
            <strong>%s (%s%%)</strong>
        </div>
        <div class="sensor">
            <span>Air Temperature</span>
            <strong>%s°C</strong>
        </div>
        <div class="sensor">
            <span>Air Humidity</span>
            <strong>%s%%</strong>
        </div>
        <div class="sensor">
            <span>Light</span>
            <strong>%s</strong>
        </div>
    </div>
  </div>
  </body>
</html>
)rawliteral";