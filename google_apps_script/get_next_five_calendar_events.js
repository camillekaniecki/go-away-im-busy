/** 
 * Based on https://github.com/googleworkspace/apps-script-samples/blob/main/calendar/quickstart/quickstart.gs
 */

/**
 * Lists 5 upcoming events in the user's calendar.
 * @see https://developers.google.com/calendar/api/v3/reference/events/list
 */

function listUpcomingEvents() {
  const calendarId = 'ckaniecki@acvauctions.com';
  // Add query parameters in optionalArgs
  const optionalArgs = {
    timeMin: (new Date()).toISOString(),
    showDeleted: false,
    singleEvents: true,
    maxResults: 5,
    orderBy: 'startTime'
    // use other optional query parameter here as needed.
  };
  try {
    // call Events.list method to list the calendar events using calendarId optional query parameter
    const response = Calendar.Events.list(calendarId, optionalArgs);
    const events = response.items;
    const allEvents = response.items;
    console.log('allEvents is', allEvents);
    if (events.length === 0) {
      console.log('No upcoming events found');
      return;
    }
    if (allEvents.length > 0) {
      console.log('should return events');
      return {
        data: allEvents,
      };
    }
    
    // Print the calendar events
    for (const event of events) {
      let when = event.start.dateTime;
      if (!when) {
        when = event.start.date;
      }
      console.log('%s (%s)', event.summary, when);
    }
  } catch (err) {
    // TODO (developer) - Handle exception from Calendar API
    console.log('Failed with error %s', err.message);
  }
}

function doGet(e) {
  const result = listUpcomingEvents();
  console.log('result is', result);
  // Your code to handle the GET request and return content
  // return HtmlService.createHtmlOutput("<div>" + JSON.stringify(result) + "</div>");
  return ContentService.createTextOutput(JSON.stringify(result)).setMimeType(ContentService.MimeType.JSON);
}
